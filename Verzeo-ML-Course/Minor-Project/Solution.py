import sys
import os.path
import pandas as pd
import matplotlib.pyplot as plt


def getAverageRatingsPerGenre(df, finalized_genre):
    AverageRatingPerGenre = []

    for g in finalized_genre:
        ratingSeries = df[df['Finalized Genre'] == g]['Average User Rating']
        if len(ratingSeries) != 0:
            AverageRatingPerGenre.append(round(ratingSeries.mean(), 2))

    data = list(zip(AverageRatingPerGenre, finalized_genre))
    return data


def getAverageAppSizePerGenre(df, finalized_genre):
    AverageAppSizePerGenre = []

    for g in finalized_genre:
        sizeSeries = df[df['Finalized Genre'] == g]['Size']
        if len(sizeSeries) != 0:
            AverageAppSizePerGenre.append(round(sizeSeries.mean(), 2))

    data = list(zip(AverageAppSizePerGenre, finalized_genre))
    return data


# def getGenreRatingsFromDF(df):
#     yAxisData = []
#     xAxisData = df['Primary Genre'].unique()
#     for data in xAxisData:
#         yAxisData.append(df[df['Primary Genre'] == data]['Average User Rating'].mean())
#
#     return [xAxisData, yAxisData]


def plotBarGraph(xAxisLabel, yAxisLabel, yxDataPoints, fontsize=10, rotation=0):
    xData, yData = [], []
    for i in yxDataPoints:
        xData.append(i[1])
        yData.append(i[0])

    plt.bar(xData, yData)
    plt.xlabel(xAxisLabel, fontsize=6)
    plt.ylabel(yAxisLabel)
    plt.xticks(fontsize=fontsize, rotation=rotation)
    plt.title('Comparison of ' + xAxisLabel + ' vs ' + yAxisLabel)
    plt.show()


def prepareData(filename):
    # Function to address the 1st objective of the mini project: clean and prepare the data to remove null values.
    print('INFO: Reading csv file...')
    # noinspection PyBroadException
    try:
        csvDF = pd.read_csv(filename)
    except FileNotFoundError:
        print('ERROR: No file found matching with name "', filename, '"! Aborting program session...', sep='')
        exit(0)
        return
    except:
        print('ERROR: Could not parse the csv file. Maybe there are incorrect column names??')
        exit(0)
        return

    # Remove unnecessary columns
    print('\nINFO: Removing unnecessary columns...')

    colsToDrop = ['URL', 'ID', 'Icon URL', 'Description', 'Original Release Date', 'Current Version Release Date',
                  'Languages', 'Primary Genre', 'Subtitle', 'Developer']
    for s in colsToDrop:
        try:
            csvDF.drop(columns=[s], inplace=True)
        except KeyError:
            print('WARNING: Parameter \'', s, '\' was not found! Ignoring...', sep='')
            continue
    # Remove rows where user rating count = 0, because it means nobody has used the app, and including it will
    # provide wrong data in average user rating.
    print('INFO: Removing rows with less-than-minimum amount of data...')
    csvDF = csvDF[pd.notnull(csvDF['User Rating Count'])]

    # Try to fill up obviously known null values
    print('INFO: Attempting to insert default values wherever possible...')
    colsToDefault = ['In-app Purchases', 'Price', 'Average User Rating', 'User Rating Count']
    for s in colsToDefault:
        try:
            if s == 'In-app Purchases' or s == 'Price':
                csvDF[s] = csvDF[s].fillna(0.00)
            elif s == 'Average User Rating' or s == 'User Rating Count':
                csvDF[s] = csvDF[s].fillna(0)
            else:
                csvDF[s] = csvDF[s].fillna('EN')
        except KeyError:
            print('\nWARNING: One of the expected parameters \'', s, '\' was not found! Ignoring...', sep='')
            continue

    # Create column to find whether app is free or paid
    csvDF['Free or Paid'] = ['Free' if price == 0 else 'Paid' for price in csvDF['Price']]

    print('\nINFO: Cleaning completed. Shape of the final dataset:', csvDF.shape)
    try:
        csvDF.to_csv('cleaned_dataset.csv')
    except PermissionError:
        print('\nERROR: Could not complete write-out of optimized dataset to disk; permission denied.')

    return csvDF


def finalizeGenres(df):
    # Function to find the least occurring genre out of the sub-genre group for each entry
    # and assign it to that entry.

    genreSeries = df['Genres']  # Select the comma-separated genre series from the column Genres
    genres = set()  # To enable selection of unique genres
    for s in genreSeries:
        genreEntryList = s.split(',')  # Obtain list of strings (genres) that are comma-separated
        for i in genreEntryList:
            genres.add(i.strip())  # Remove white-spaces at the start or end of the string.

    genres = list(genres)  # Convert back to list to make it iterable and accessible by index

    genreFreqData = [0] * len(genres)  # To store how many times a particular genre occurs in the Series

    # For each genre in the list, check if it occurs in each entry of the Series. If it does, then increment
    # the corresponding index in the freqs array by 1.
    for i in range(len(genres)):
        for entry in genreSeries:
            if genres[i] in entry:
                genreFreqData[i] += 1

    # List to append to the CSV file as a Series, containing the least frequently occurring genre for each
    # comma-separated set of genres for each entry in the Series.
    finalized_genre = []

    # Calculating least occurring genre in each entry by comparing the frequencies of each genre in the entry,
    # and then finding the minimum frequency and hence the corresponding genre.

    for entry in genreSeries:
        minfreq = max(genreFreqData)
        minGenre = ''
        for i in range(len(genreFreqData)):
            if genres[i] in entry:
                if genreFreqData[i] < minfreq:
                    minGenre = genres[i]
                    minfreq = genreFreqData[i]
        finalized_genre.append(minGenre)

    df['Finalized Genre'] = finalized_genre  # Append to CSV file
    # noinspection PyBroadException
    try:
        df.to_csv('reducedDataset_Final.csv')
    except PermissionError:
        print('ERROR: Could not write the reduced dataset file to disk; permission denied! Is the file open in some '
              'other process?')
        print('INFO: Continuing program execution without writing to disk...')
    except:
        print('ERROR: Could not write the reduced dataset to disk! Continuing program execution...')
    # Select all unique values from the list and return as list (iterable form)
    finalized_genre = set(finalized_genre)
    return list(finalized_genre)


def getPlotDataFromDF(df):
    data = sorted(df['Price'].unique().tolist())
    print('   Price\tNo. of Apps with this Price\tMean of the Average User Rating')
    print('-' * 75)
    averageRating = []
    for pr in data:
        x = df[df['Price'] == pr]['Average User Rating']
        averageRating.append(round(x.mean(), 2))
        print('', str(pr).rjust(6, ' '), '\t', str(len(x)).rjust(16, ' '), '\t', str(averageRating[-1]).rjust(20, ' '))
    print('\n\nWARNING: The no. of records in the dataset that are in each price level may vary, and this impact the '
          'average. '
          'The above tabular column has been printed for your reference, for you to consider the number of records in '
          'each price level and decide its weight appropriately.')
    return [data, averageRating]


def displayPlot(xLabel, yLabel, data):
    plt.plot(data[0], data[1])
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)
    plt.title('Variation of ' + xLabel + ' vs. ' + yLabel)
    plt.show()


def printGenresWithHigherUserRatings(genresPopularityList):
    ratings = sorted(genresPopularityList, reverse=True)
    n = input('\nEnter the no. of top-ranking genres to display: ')
    if not (n.isnumeric()):
        print('\nWARNING: Numeric data not received! Defaulting to 5...')
        n = 5
    else:
        n = int(n)
        if n > len(genresPopularityList) or n <= 0:
            print('\nWARNING: Selected value is not within the range of accepted values (0,', len(genresPopularityList),
                  ']! Defaulting to ', len(genresPopularityList), '...', sep='')
            n = len(genresPopularityList) - 1
    print('The top', n, 'genre(s) are: ')
    for i in range(n):
        print(i + 1, ':', ratings[i][1], '( Rating:', ratings[i][0], ')')


def getPriceHistDataFromDF(df):
    prices = sorted(df['Price'].unique().tolist())
    data = []
    for pr in prices:
        for x in df[df['Price'] == pr]['Price']:
            data.append(x)
    return data


def plotHist(xLabel, yLabel, data):
    plt.hist(data, bins=200)
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)
    plt.title('Histogram of ' + xLabel + ' vs. ' + yLabel)
    plt.show()
    pass


def main():
    if len(sys.argv) < 2 or sys.argv[1][-3:] != 'csv':
        print('Usage: ', os.path.basename(__file__), ' <CSVFileName>.csv.')

        return
    print('\nINFO: Preparing dataset...')
    cleanedDF = prepareData(sys.argv[1])  # addresses objective 1.

    print('\nINFO: Calculating average rating for each genre...')
    finalizedGenreList = finalizeGenres(cleanedDF)  # The next 4 lines
    genresPopularity = getAverageRatingsPerGenre(cleanedDF, finalizedGenreList)  # jointly address objective 2
    print('INFO: Plotting graph of Genre vs Average Rating/Genre...')  # of plotting the bar graph of genre vs rating
    plotBarGraph('Finalized Genre', 'Average Rating per Genre', genresPopularity, fontsize=5, rotation=60)

    # To address objective 3 of identifying genres with higher user ratings
    printGenresWithHigherUserRatings(genresPopularity)
    print('\nINFO: Analyzing variation of price range with no. of apps...')
    plotHist('Price Range (in $)', 'No. of apps', getPriceHistDataFromDF(cleanedDF))
    print('\nINFO: Analyzing variation of rating with average price for each rating...')  # The next 4 lines will
    plotData = getPlotDataFromDF(cleanedDF)  # jointly address
    print('\nINFO: Plotting bar graph of price vs average user rating...')  # objective 4 of identifying the trend
    displayPlot('Price Range (in $)', 'Average User Rating', plotData)  # of price of the apps vs user rating.

    # The notepad file will address the final objective of stating my inferences from the given dataset.
    print('\n\nThe program execution has completed successfully. Please refer to the text file along with this Python '
          'script for inferences that can be made from the given dataset in the question, appstore_games.csv.')


if __name__ == "__main__":
    main()
