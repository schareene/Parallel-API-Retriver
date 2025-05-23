## Parallel API Stock Market Retriver (QuotaAPI)

## About

An API Stock Market Request program. Allows users to see the real-time stock market information, consisting with the use of ALPHA Vantage API and parallel computing with OpenMP. The purpose of the program is to serve users who are beginners at understanding how the Stock Market functions. Therefore, users have the ability to enter the ticker symbols from each company they select, retrive infromation from the Open Price, High Price, Low Price, Current Price, Latest Trading Day, Previous Closing Day, Change Price, and Change Percentage for users to make a decision to BUY or SELL based on how the market looks like.

## Installation/ Prerequisites:
You will first need to install the following software/libraries.

-g++ (GCC Compiler with OpenMP)
-cURL (for API requests)
-nlohmann/json library (for JSON parsing)
-Alpha Vantage API Key (Get one at [Alpha Vantage](https://www.alphavantage.co/support/#api-key))

Installation Commands (Linux/macOS/WSL)

sudo apt update && sudo apt install g++ libcurl4-openssl-dev libomp-dev nlohmann-json3-dev -y

For macOS:
brew install gcc curl libomp nlohmann-json

For Windows:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl mingw-w64-x86_64-openmp

## To Compile/Run the program:
g++ QuotaAPI.cpp -o Quota_API -fopenmp -lcurl -lstdc++fs -I/usr/include/nlohmann
./Quota_API

## Sources used:
To retrieve the Stock Market API of the ticker symbols: 
https://www.alphavantage.co/

## Built with
[VSCode] (https://code.visualstudio.com/)
[Postman] (https://www.postman.com/) - API testing software

## References
[1] "OpenMP Parallel Programming Full Course", Scientific Programming School https://www.youtube.com/watch?v=FjCFT5ojQSk

[2] "Stock Market & Deep Learning 2019" dProgrammer Lopez https://www.youtube.com/watch?v=jugZq88kvAU&list=TLPQMTkwMzIwMjVaZDRCo8rA9Q&index=4

[3] "OpenMP offload optimization guide: beyond kernels -Lessons learned in QMCPACK", OpenMP https://www.youtube.com/watch?v=iPGMYVViQzM

[4] "Getting the Interactive Brokers API working in 8 minutes", Rachel Doji https://www.youtube.com/watch?v=LofU4nHZumo&list=TLPQMTkwMzIwMjWtJD5c1FOGyw&index=4

[5] "Streamlit STOCK dashboard using Python, Financial Programming with Ritvik", CFA, FRM https://www.youtube.com/watch?v=9GA2WlYFeBU&t=1s

[6] Quantum Task Offloading with the OpenMP API, Lee J., Bull M., Rueffenacht M., Doerfert J., Klemm M., Schulz M.
https://arxiv.org/pdf/2311.03210
