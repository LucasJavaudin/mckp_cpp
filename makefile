# Object files to either reference or create                          
OBJECTS = data.o mckp.o ExhaustiveSearch.o DiscreteAlgorithm.o GreedyAlgorithm.o DyerZemel.o
# The executable file that will be created at the end                 
EXEC = run
# The code compiler to use for compilation                            
CC = g++                                                              

# Perform action on all object files (May or may not exist)           
all: $(OBJECTS)                                                       
	$(CC) -o $(EXEC) $(OBJECTS)
