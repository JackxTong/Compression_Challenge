To use C++:

```bash
g++ -std=c++17 ./src/Main.cpp ./src/wavReader.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lboost_program_options -o bin/my_program
```

then

```bash
./bin/my_program --ngram 3
```

should print
```
READING 743 FILES ...
TOTAL COUNT FOUND: 73382431

Sum of all probabilities: 1 , num symbols = 1427664
READING TERMINATED
```

and saved the probabiltiy dictionary into `ngram_probabilities.txt`

To use python:

```bash
python3 process_wav.py --ngram 3
```

should print
```
Reading files...

Sum of all probabilities: 0.9999999999838164
Number of unique symbols: 1427664
```

and saved the probabiltiy dictionary into `ngram_prob_python.txt`

Given this dict, can run `arithmetic.py` for arithmetic coding.
