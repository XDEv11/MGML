SOURCE = MGML.cpp
PYWRAP = pywrap.cpp
OBJ = MGML.o
PYSO = MGML$(shell python3-config --extension-suffix)
CPPTEST = MGML_test
PYTEST = MGML_test.py

all: cpp py

cpp: $(OBJ)
	g++ -Wall -std=c++17 $(CPPTEST).cpp $(OBJ) -o $(CPPTEST)
	./$(CPPTEST)

py: $(PYSO)

$(OBJ): $(SOURCE)
	g++ -O3 -Wall -std=c++17 -c $(SOURCE) -o $(OBJ)

$(PYSO): $(PYWRAP)
	g++ -O3 -Wall -std=c++17 -shared -fPIC $(PYWRAP) $(OBJ) -o $(SO) \
		$(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags)

clean:
	rm -f $(OBJ) $(CPPTEST) $(PYSO)

.PHONY: all cpp py clean
