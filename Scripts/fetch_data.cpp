// Include files
#include<SerialStream.h>
#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include<iomanip>
//##########################

// Function returns formated file name: YY.MM.DD
std::string get_file_name(std::tm* time) {

    int year = time->tm_year + 1900;
    int month = time->tm_mon + 1;
    int day = time->tm_mday;   

    std::string date_formated = std::to_string(year) + "." 
                              + std::to_string(month) + "."
                              + std::to_string(day);

    return date_formated;
}

// Function returns current time
std::string get_time(std::tm* time) {

    int hour = time->tm_hour;
    int minute = time->tm_min;
    int second = time->tm_sec;

    std::string time_formated = std::to_string(hour) + ":" 
                              + std::to_string(minute) + ":"
                              + std::to_string(second) + "::";            
    
    return time_formated;
}

// Execute a command and return result. Can be used for other commands aswell
std::string executeCommand(const char* cmd) {
    // Open a pipe to the command
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        std::cerr << "Error: Unable to open pipe." << std::endl;
        return "";
    }

    // Read the command output into a buffer
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    // Close the pipe
    pclose(pipe);

    return result;
}

int main()
{
    // Define Constants
    const char* serial_port = "/dev/ttyACM0";
    const char* CPU_temp_command = "vcgencmd measure_temp";
    const char* request_commands_command = "wget --quiet -O test.txt 192.168.1.221:3000/commands";
    const char* current_data_file = "../Status_bars/data.txt";
    
    //Define variables
    LibSerial::SerialStream serial_stream(serial_port);
    std::string serial_data;
    std::string temperature;
    
    // Check if serial port was opened. Make sure serial_port is pointing to the arduino serial port.
    if (!serial_stream.IsOpen())
    {
        std::cerr << "Error: Could not open serial port." << std::endl;
        return 1;
    }

    // Set serial port parameters
    // BaudRate should correspond to the arduino's Serial.begin() baud rate.
    serial_stream.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
    serial_stream.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    serial_stream.SetParity(LibSerial::Parity::PARITY_NONE);
    serial_stream.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    serial_stream.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);

    // Loop through the same program
    while (true)
    {
        // Check for commands as the main priority
        executeCommand(request_commands_command);


        // Read a line from the serial port
        getline(serial_stream, serial_data);

        // Get current time
        std::time_t current_time = std::time(nullptr);
        std::tm* local_time = std::localtime(&current_time);

        // Save to Data directory, which should be located in Desktop
        std::string saved_data_file = "../../Data/" + get_file_name(local_time) + ".txt";

        // Open files to save the data
        std::ofstream saved_output_file(saved_data_file, std::ios::app);
        std::ofstream current_output_file(current_data_file);

        // Get CPU temperature and format it
        temperature = executeCommand(CPU_temp_command);
        temperature = temperature.substr(temperature.find('=') + 1, temperature.find('C') - temperature.find('=') - 2);

        if (saved_output_file.is_open()) {
            saved_output_file << get_time(local_time) << "5|" << temperature << "|" << serial_data;
            saved_output_file.close();
        }

        if(current_output_file.is_open()) {
            current_output_file << "5" << std::endl;
            current_output_file << temperature << std::endl;
            current_output_file << serial_data;
            current_output_file.close();
        }
    }

    // Close the serial port (this part may not be reached in this example)
    serial_stream.Close();

    return 0;
}
