// in thsis project i will use statistics of to predict whether a day with given data points will have
// rain or not. there is no ML involved right now but at least i am taking the first step

#include <stdio.h>
#include <string.h>
#include <math.h>

struct rain_prediction
{

    int day_number;
    float temp; // in degree celcius as main model will be fine-tuned to predict rain in Jaipur
    float humidity;

    float pressure; // in atm
    float precipitation_in_mm;
    // note that i will be using CSV format for my rain-history.txt record files
};

void printing_entire_dataset()
{
    FILE *fp;
    fp = fopen("rain_history.txt", "r");

    if (fp == NULL) // this time i've added a file presence validator which earlier i did not do
    {
        printf("\nRecord file not found.");
        return;
    }
    char lines[1000];
    int day_count = 0;
    float years;
    while (fgets(lines, 1000, fp))
    {
        printf("%s\n", lines);
        day_count++;
    }
    years = (float)day_count / 365.00;
    printf("\n \n The total number of days present in data = %d and approximately our data is based on past   %.2f", day_count, years);
    printf(" years");
    fclose(fp);
}

void will_it_rain()
{
    float current_pressure, current_humidity, current_avg_temp;

    printf("Enter the required day's  average temperature (in degree Celcius): \n");
    scanf("%f", &current_avg_temp);
    printf("Enter required day's humidty: \n");
    scanf("%f", &current_humidity);
    printf("\nEnter the required day's pressure: \n");
    scanf("%f", &current_pressure);

    /* Note that in our data, day indexis a natural no., temperature is in degree Celcius
    humidity is described as a %, pressure has the unit of hPa (hectopascals) and
    precipitation is shown in millimeters.
    */

    FILE *fp;
    fp = fopen("rain_history.txt", "r");

    struct rain_prediction rainYN; // rainYN stands as an abbreviation of rain yes or no;
    int matched_days = 0;

    // now using the KNN principle to find 50 closest days

    // in KNN we use euclidean distnace here.

    double eucl_distance;
    double arr_eucl[4018];
    double precipiation_amt[4018]; // array to store the precipitaion amount in mm;

    int i = 0;

    while (fscanf(fp, "%d,%f,%f,%f,%f", &rainYN.day_number, &rainYN.temp, &rainYN.humidity, &rainYN.pressure, &rainYN.precipitation_in_mm) == 5)
    {
        // applying KNN here
        // first we find euclidean distance of all the 4018 values wrt our input data-point
        eucl_distance = sqrt(((current_avg_temp - rainYN.temp) * (current_avg_temp - rainYN.temp)) + ((current_humidity - rainYN.humidity) * (current_humidity - rainYN.humidity)) + ((current_pressure - rainYN.pressure) * (current_pressure - rainYN.pressure)));
        // thus calculated euclidean distance per data value;
        // storing the euclidean distances in an array to later sort it in ascending order
        arr_eucl[i] = eucl_distance;
        // also storing the corresponding precipiatation value in an array;
        precipiation_amt[i] = rainYN.precipitation_in_mm;
        i++;
    }
    // now printing the array of euclidean distances for checking
    for (int i = 0; i < 4018; i++)
    {
        printf("%.2f ,", arr_eucl[i]);

    } // therefore yes the euclidean distances  are properly stored in an array

    printf("\n\n\n Precipiation amount array\n\n");
    for (int k = 0; k < 4018; k++)
    {
        printf("%.2f ,", precipiation_amt[k]);
    }

    // now sorting the euclidean distance array;
    float dist_temp;
    float precipitation_temp;

    for (int i = 0; i < 4018; i++)
    {
        for (int j = i + 1; j < 4018; j++)
        {
            if (arr_eucl[i] > arr_eucl[j])
            {
                // swapping the euclidean distances here to sort them in ascending order.
                dist_temp = arr_eucl[i];
                arr_eucl[i] = arr_eucl[j];
                arr_eucl[j] = dist_temp;

                // now swapping precipiation values here
                // so that if a distance moves places, so does its corresponding rainfall value
                precipitation_temp = precipiation_amt[i];
                precipiation_amt[i] = precipiation_amt[j];
                precipiation_amt[j] = precipitation_temp;
                // now that both arrays , one having euclidean distances and other having
                // precipitation amts are sorted to pair each other; note that
                // the precipitation array is not at all sorted ascending order wise.
            }
        }
    }

    // now printing the sorted array

    printf("\n\n\n\n\n The sorted euclidean distance array is: \n\n\n");
    for (int i = 0; i < 4018; i++)
    {
        printf("%.2f ,", arr_eucl[i]);
    } // so yes , both myy raw array and ascending order sorted array are working and printing just fine.
    // now using the sorted array to get values of first 50 values, index 0 to index 49;
    double copy_arr[50];
    for (int i = 0; i < 50; i++)
    {
        copy_arr[i] = arr_eucl[i];
    }
    printf("\n\n\n\n\n\n Now printing the copied array which contains first 50 values of euclidean distances: \n\n\n");
    for (int i = 0; i < 50; i++)
    {
        printf("%.2f ,", copy_arr[i]);
    } // hence the first 50 values are also printed fine ✅✅✅

    printf("\n\n\n\n\n\n Now printing the precipitation array which contains first 50 values: \n\n\n");
    for (int i = 0; i < 50; i++)
    {
        printf("%.2f ,", precipiation_amt[i]);
    } // hence the first 50 values are also printed fine ✅✅✅

    // now the real engine begins
    int rainy_day_count = 0;
    for (int j = 0; j < 50; j++)
    {
        if (precipiation_amt[j] >= 0.75)
        {
            rainy_day_count++;
        }
    }
    double percentage = ((float)rainy_day_count / 50) * 100;
    printf("\n\n\n\n The chances of precipitation based on input data is: %.2f", percentage);

    int k = 0;
    printf("\n\n");
}

int main()
{
    int user_input;
    while (1)
    {
        printf("\n\nView Records: 1\n");
        printf("\nPredict rainfall percentage: 2\n");
        scanf("%d", &user_input);
        if (user_input == 1)
        {
            printing_entire_dataset();
        }
        else if (user_input == 2)
        {
            will_it_rain();
        }
        else
        {
            printf("\n Invalid Response.\n");
        }
    }

    return 0;
}