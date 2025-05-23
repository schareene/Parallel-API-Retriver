#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <omp.h>  

using namespace std;
using json = nlohmann::json;

const string API_KEY = "YOUR_API_KEY"; // Used in the getStockPrices.
const int THREAD_SLEEP_MS = 200; // Helps prevent API rate limits.
const int MAX_THREADS = omp_get_max_threads();

size_t ResponseCallback(void* contents, size_t size, size_t nmemb, string* output)
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize; // Callback function to get the cURL response from the API.
}

string marketData(const string& ticker)
{
    CURL* curl;
    CURLcode res;
    string response;
    string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + ticker + "&apikey=" + API_KEY;

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: StockSim/1.0");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            cerr << "There's an error from the API request for " << ticker << ": " << curl_easy_strerror(res) << endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return response;
}

void getStockPrices(const vector<string>& tickers, vector<json>& stockData) 
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (size_t i = 0; i < tickers.size(); i++) 
            {
                #pragma omp task firstprivate(i)
                {
                    int thread_num = omp_get_thread_num();

                    #pragma omp critical // Only one thread can print at a time to avoid overleading.
                    {
                        cout << "Thread " << thread_num << " obtaining market data for: " << tickers[i] << endl;
                    }
                    string data = marketData(tickers[i]);

                    try 
                    {       
                        json jsonData = json::parse(data);
                        if (jsonData.contains("Global Quote"))
                        {
                            #pragma omp critical
                            {
                                stockData[i] = jsonData["Global Quote"];
                            }
                        } 
                        else 
                        {
                            #pragma omp critical
                            {
                                cerr << "Thread " << thread_num << " This is an invalid ticker: " 
                                     << tickers[i] << ". Please double check the ticker symbol is correct." << endl;
                            }
                        }
                    } 
                    catch (const json::parse_error& e) 
                    {
                        #pragma omp critical
                        {
                            cerr << "Thread " << thread_num << " JSON interpreting the data has failed for " 
                                 << tickers[i] << ": " << e.what() << endl;
                        }
                    }

                    this_thread::sleep_for(chrono::milliseconds(200));
                }
            }
        }
    }
}

void displayStockData(const vector<string>& tickers, const vector<json>& stockData)
{

    #pragma omp parallel for
    for (size_t i = 0; i < tickers.size(); i++)
    {
        int thread_id = omp_get_thread_num();
        cout << "\nThread(s) " << thread_id << " for " << tickers[i] << endl;
        try
        {
            string open_price = stockData[i]["02. open"].get<string>();
            string high_price = stockData[i]["03. high"].get<string>();
            string low_price = stockData[i]["04. low"].get<string>();
            string current_price = stockData[i]["05. price"].get<string>();
            string latest_trading_day = stockData[i]["07. latest trading day"].get<string>();
            string previous_close = stockData[i]["08. previous close"].get<string>();
            string change = stockData[i]["09. change"].get<string>();
            string change_percent = stockData[i]["10. change percent"].get<string>();

            double currentPrice = stod(current_price);
            double previousClosePrice = stod(previous_close);
            string price_change_indicator = (currentPrice > previousClosePrice) ? "UP" : "DOWN";

            cout << "\nStock Data for:\n" << tickers[i] << endl;
            cout << "Open Price: $" << open_price << endl;
            cout << "High Price: $" << high_price << endl;
            cout << "Low Price: $" << low_price << endl;
            cout << "Current Price: $" << current_price << " (" << price_change_indicator << ")" << endl;
            cout << "Latest Trading Day: " << latest_trading_day << endl;
            cout << "Previous Closing Day Price: $" << previous_close << endl;
            cout << "Change: $" << change << endl;
            cout << "Change %: " << change_percent << "%" << endl;
        }
        catch (const json::type_error& e)
        {
            cerr << "No data found for " << tickers[i] << ". Cannot retrieve for its market data." << endl;
        }
        catch (const exception& e)
        {
            cerr << "Error with the market data for " << tickers[i] << ": " << e.what() << endl;
        }
    }
}
int main()
{
    int numTickers;
    cout << "What information are you looking for?" << endl;
    cout << "Enter the number of companies you would like to see: ";
    cin >> numTickers;

    vector<string> tickers(numTickers);
    vector<json> stockData(numTickers);

    for (int i = 0; i < numTickers; i++)
    {
        cout << "Enter the ticker symbol for company #" << (i + 1) << ": ";
        cin >> tickers[i];
    }
    cout << "\nRetrieving the stock market.\n" << endl;
    getStockPrices(tickers, stockData);
    displayStockData(tickers, stockData);
    return 0;
}
