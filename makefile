CC = g++-4.8
FLAGS = -std=c++11
BIN_DIR = bin
INC_DIR = include
BUILD_DIR = build
SRC_DIR = src
OBJECTS = $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/User.o
INCLUDE = -I./$(INC_DIR)

$(BIN_DIR)/Agenda_ex:$(OBJECTS)
	$(CC) $(FLAGS) $(INCLUDE) $(OBJECTS) -o $(BIN_DIR)/Agenda_ex

$(BUILD_DIR)/Agenda.o:$(SRC_DIR)/Agenda.cpp $(BUILD_DIR)/AgendaUI.o
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/Agenda.cpp -o $(BUILD_DIR)/Agenda.o

$(BUILD_DIR)/AgendaUI.o:$(SRC_DIR)/AgendaUI.cpp $(BUILD_DIR)/AgendaService.o
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/AgendaUI.cpp -o $(BUILD_DIR)/AgendaUI.o

$(BUILD_DIR)/AgendaService.o:$(SRC_DIR)/AgendaService.cpp $(BUILD_DIR)/Storage.o
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/AgendaService.cpp -o $(BUILD_DIR)/AgendaService.o

$(BUILD_DIR)/Storage.o:$(SRC_DIR)/Storage.cpp $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Date.o
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/Storage.cpp -o $(BUILD_DIR)/Storage.o

$(BUILD_DIR)/Meeting.o:$(SRC_DIR)/Meeting.cpp $(BUILD_DIR)/Date.o
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/Meeting.cpp -o $(BUILD_DIR)/Meeting.o

$(BUILD_DIR)/Date.o:$(SRC_DIR)/Date.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/Date.cpp -o $(BUILD_DIR)/Date.o

$(BUILD_DIR)/User.o:$(SRC_DIR)/User.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o
	
clean:
	rm $(BIN_DIR)/TEST
	rm $(OBJECTS)