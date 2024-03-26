// Include files
#include<SerialStream.h>
#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<chrono>
#include<cstring>
#include <vector>
#include <sstream>
//##########################

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
    const char* request_commands_command = "wget --quiet -O ~/Desktop/hydroponisk_odling/Scripts/cmd.txt 192.168.1.221:3000/commands";
    const char* current_data_file = "../Status_bars/Host/data.txt";

    //Define variables
    LibSerial::SerialStream serial_stream(serial_port);
    std::string temperature;
    std::string serial_data;
    
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
        // Get current time for file_name and time
        auto currentTime = std::chrono::system_clock::now();
        char file_name_buffer[20];
        char time_buffer[20];

        auto transformed = currentTime.time_since_epoch().count() / 1000000;
        auto millis = transformed % 1000;

        std::time_t tt;
        tt = std::chrono::system_clock::to_time_t ( currentTime );
        auto timeinfo = localtime (&tt);

        // Formats the file_name and time
        strftime (file_name_buffer,20,"%F",timeinfo);
        strftime (time_buffer,20,"%H.%M.%S",timeinfo);
        sprintf(time_buffer, "%s:%03d",time_buffer,(int)millis);

        // Get data from arduino
        getline(serial_stream, serial_data);
	

	// Splits serial_data at the space
	std::stringstream test(serial_data);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, ' '))
	{
	   seglist.push_back(segment);
	}

        // Fetch data from Server
        //executeCommand(request_commands_command);
        
        // Save to Data directory, which should be located in Desktop
        std::string saved_data_file = "../../Data/" + std::string(file_name_buffer) + ".txt";

        // Open files to save the data
        std::ofstream saved_output_file(saved_data_file, std::ios::app);
        std::ofstream current_output_file(current_data_file);

        // Get CPU temperature and format it
        temperature = executeCommand(CPU_temp_command);
        temperature = temperature.substr(temperature.find('=') + 1, temperature.find('C') - temperature.find('=') - 2);

        if (saved_output_file.is_open()) {
            saved_output_file << std::string(time_buffer) << "::" << seglist[0] << "|"  << temperature << "|" << seglist[1] << std::endl;
            saved_output_file.close();
        }

        if(current_output_file.is_open()) {
            current_output_file << seglist[0] << std::endl << temperature << std::endl << seglist[1] << std::endl;
            current_output_file.close();
        }
    }

    // Close the serial port (this part may not be reached in this example)
    serial_stream.Close();

    return 0;
}
