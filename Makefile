SOURCE = MGML.cpp
PYWRAP = pywrap.cpp
OBJ = MGML.o
PYSO = MGML$(shell python3-config --extension-suffix)
CPPTEST = test_MGML
PYTEST = test_MGML_correctness.py
GEN = gen_MGML

all: cpp py gen

cpp: $(OBJ) $(CPPTEST)
	@echo "Testing in C++..."
	@./$(CPPTEST)

$(CPPTEST): $(CPPTEST).cpp
	g++ -Wall -std=c++17 $(CPPTEST).cpp $(OBJ) -o $(CPPTEST)

py: $(PYSO)
	@echo "Testing in Python..."
	@python $(PYTEST) 1>test.log
	@cat test.log

gen: $(GEN)

$(GEN): $(GEN).cpp
	g++ -Wall -std=c++17 $(GEN).cpp -o $(GEN)

$(OBJ): $(SOURCE)
	g++ -O3 -Wall -std=c++17 -c $(SOURCE) -o $(OBJ)

$(PYSO): $(PYWRAP)
	g++ -O3 -Wall -std=c++17 -shared -fPIC $(PYWRAP) $(OBJ) -o $(PYSO) \
		$(shell python3 -m pybind11 --includes) $(shell python3-config --includes --ldflags)

clean:
	rm -f $(OBJ) $(CPPTEST) $(PYSO) $(GEN)
	rm -rf __pycache__

.PHONY: all cpp py clean
