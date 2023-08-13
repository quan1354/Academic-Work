#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define __CL_ENABLE_EXCEPTIONS

//Include
#include <CL/cl.hpp>
#include "Utils.h"
#include <iostream>
#include <vector>
#include "Weather.h"
#include <ctime>
#include <algorithm>
#include <map>
#include <chrono>

//Include custom
#include "SerialStatistics.h"
#include "WriteFile.h"

//Define MyType
typedef float myType;

//Declarations of functions
void Serial(std::vector<float>& Values);
void Parallel(std::vector<float>& Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event);
myType SumVec(std::vector<myType>& temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event);
myType STDVec(std::vector<myType>& temp, myType Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event);
std::vector<myType> Sort(std::vector<myType>& temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event);
int AddPadding(std::vector<myType>& temp, size_t LocalSize, float PadVal);
void KernalExec(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name);
float KernalExecRet(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name);

void saveYearAndMonths(std::vector<int>);
std::vector<myType> call_kernel(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name);
void Histogram(std::vector<float>& Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event, float maximum, float minimum);

// Global Variable
int currentMonth;
int inputYear;
bool isValid = false;
bool isOverall;
unsigned long serialTotalTime = 0;
unsigned long parallelTotalTime = 0;

std::string monthName[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL",
							"MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER",
							"OCTOBER", "NOVEMBER", "DECEMBER" };

//Print help function
void print_help(){
	//Display help message
	std::cout << "Application usage:" << std::endl;
	std::cout << "  -p : select platform " << std::endl;
	std::cout << "  -d : select device" << std::endl;
	std::cout << "  -l : list all platforms and devices" << std::endl;
	std::cout << "  -h : print this message" << std::endl;
}

//Main method
int main(int argc, char **argv) {
	//Load Weather Data
	Weather Data = Weather();
	Data.Load("temp_lincolnshire_short.txt");

	//Get the temperature data from the weather data
	std::vector<float>& temp = Data.GetTemp();
	std::vector<int>& year = Data.GetYear();
	std::vector<int>& month = Data.GetMonth();
	std::vector<string>& location = Data.GetName();

	std::vector<float> myTemp;
	std::vector<int> myMonth;

	//Handle command line options such as device selection, verbosity, etc.
	int platform_id = 1;
	int device_id = 0;
	//Loop over arguments and perform functions
	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-p") == 0) && (i < (argc - 1))) { platform_id = atoi(argv[++i]); }
		else if ((strcmp(argv[i], "-d") == 0) && (i < (argc - 1))) { device_id = atoi(argv[++i]); }
		else if (strcmp(argv[i], "-l") == 0) { std::cout << ListPlatformsDevices() << std::endl; }
		else if (strcmp(argv[i], "-h") == 0) { print_help(); }
	}
	//Try to get all the relecant information ready
	try
	{
		//Get the context
		cl::Context context = GetContext(platform_id, device_id);
		//Display what platform and device the code is running on
		std::cout << "Runinng on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl << std::endl;
		//Get the queue
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);
		//Setup the sources
		cl::Program::Sources sources;
		//Link to the kernals.cl document
		AddSources(sources, "my_kernels.cl");
		//Define the program with the context and sources
		cl::Program program(context, sources);
		//Setup prof Event
		cl::Event prof_event;

		//Try to build the program
		try 
		{
			program.build();
		}
		catch (const cl::Error& err) 
		{
			//Else display error messages
			std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}

		do {
			std::cout << "Please enter a year: ";
			std::cin >> inputYear;

			// calculate overall statistical summary
			std::cout << "=================Overall Summary================" << std::endl;
			isOverall = true;
			Serial(temp);
			Parallel(temp, context, queue, program, prof_event);
			isOverall = false;
			std::cout << "==================================================" << std::endl;

			// Check if the year exists in the vector
			auto it = std::find(year.begin(), year.end(), inputYear);
			if (it != year.end()) {
				std::cout << "The year " << inputYear << " exists in the available years." << std::endl;
				isValid = true;
				
				// take the entered year of data
				for (int i = 0; i < year.size() - 1; i++) {
					if (year[i] == inputYear) {
						myTemp.push_back(temp[i]);
						myMonth.push_back(month[i]);
					}
				}

				// To take all unique month at the year
				std::vector<int> uniqueMonth = myMonth;
				std::sort(uniqueMonth.begin(), uniqueMonth.end());
				auto last = std::unique(uniqueMonth.begin(), uniqueMonth.end());
				uniqueMonth.erase(last, uniqueMonth.end());

				// save the year and months that collected into file
				saveYearAndMonths(uniqueMonth);

				std::vector<float> placerTemp;

				// calculate statistical summary for each month of the year
				for (int i = 0; i < uniqueMonth.size(); i++) {
					currentMonth = uniqueMonth[i]; // use as global variable
					std::cout << "===========Current Month:" << monthName[currentMonth - 1] << "=============" << std::endl;
					for (int j = 0; j < myMonth.size(); j++) {
						// copy same month of temperature data
						if (currentMonth == myMonth[j]) {
							placerTemp.push_back(myTemp[j]);
						}
					}
					// perform calculation
					Serial(placerTemp);
					Parallel(placerTemp, context, queue, program, prof_event);
					// clear vector
					placerTemp.clear();
				}
				// Print overall serial execution time
				std::cout << "Serial execution time: " << serialTotalTime << " ms" << std::endl;
				// Print overall parallel execution time
				std::cout << "Parallel execution time: " << parallelTotalTime << " ms" << std::endl;
			}
			else {
				std::cout << "The year " << " does not exist in the available years. Please Try Again." << std::endl;
				isValid = false;
			}
		} while (!isValid);
	}
	catch (cl::Error err) 
	{
		//Else display error message
		cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << endl;
	}
	//Pause the console
	std::cout << "\nPress Enter to Terminate...";
	//getchar();
	 system ("pause");
	//Return 0
	return 0;
}

void Serial(std::vector<float>& Values) {
	SerialStatistics SStats = SerialStatistics();
	int Size = Values.size();

	std::cout << "==================Serial Run Time=========================" << std::endl;
	//Sort the value into ascending and display the execution time
	auto begin = std::chrono::high_resolution_clock::now();
	std::vector<float> sortedValues = SStats.insertionSort(Values, ASCENDING);
	auto end = std::chrono::high_resolution_clock::now();
	auto time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	//for (auto num : sortedValues) {
	//	printf("%.2f\n", num);
	//}
	std::cout << "Sort function execution time [ns]:" << time1 << std::endl;

	//Sum the vector and display the execution time
	begin = std::chrono::high_resolution_clock::now();
	float Sum = SStats.Sum(sortedValues);
	end = std::chrono::high_resolution_clock::now();
	auto time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	std::cout << "Sum function execution time [ns]:" << time2 << std::endl;

	//Calculate mean and display the execution time
	begin = std::chrono::high_resolution_clock::now();
	float mean = SStats.Mean(sortedValues);
	end = std::chrono::high_resolution_clock::now();
	auto time3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	std::cout << "Mean function execution time [ns]:" << time3 << std::endl;

	//Calculate standard deviation and display the execution time
	begin = std::chrono::high_resolution_clock::now();
	float sd = SStats.StandardDeviation(sortedValues);
	end = std::chrono::high_resolution_clock::now();
	auto time4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	std::cout << "StandardDeviation function execution time [ns]:" << time4 << std::endl;

	//Get min and max value
	float min = sortedValues[0];
	float max = sortedValues[sortedValues.size() - 1];

	//Calculate median and display the execution time
	begin = std::chrono::high_resolution_clock::now();
	float median = SStats.GetMedianValue(sortedValues);
	end = std::chrono::high_resolution_clock::now();
	auto time5 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	std::cout << "GetMedianValue function execution time [ns]:" << time5 << std::endl;

	//Display the total execution time
	auto totalTime = time1 + time2 + time3 + time4 + time5;
	serialTotalTime += totalTime;
	std::cout << "Total execution time [ns]:" << totalTime << std::endl << std::endl;

	std::cout << "-------------------SERIALS RESULT----------------------" << std::endl;
	std::cout << "The Sum is: " << Sum << std::endl;
	std::cout << "the mean is: " << mean << std::endl;
	std::cout << "the standard deviation is: " << sd << std::endl;
	std::cout << "the min is: " << min << std::endl;
	std::cout << "the max is: " << max << std::endl;
	std::cout << "the median value is: " << median << std::endl;
	std::cout << "------------------------------------------------" << std::endl << std::endl;

	std::cout << "----------------HISTOGRAM RESULTS FREQUENCY PER BIN----------------" << std::endl;
	int bin_count = 5;
	float class_width = (max - min) / bin_count;

	//Call histogram function
	begin = std::chrono::high_resolution_clock::now();
	vector<int> hist = SStats.hist_serial(sortedValues, bin_count, min, max, class_width);
	end = std::chrono::high_resolution_clock::now();
	auto time6 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

	std::cout << "Minimum: " << min << ", Maximum: " << max << std::endl;
	std::cout << "Number of Bins: " << bin_count << ", Bin Size: " << class_width << std::endl;
	for (int i = 0; i < bin_count; i++) {
		float binStart = min + i * class_width;
		float binEnd = min + (i + 1) * class_width;
		std::cout << "Bin Range: " << binStart << " to " << binEnd << ", Frequency: " << hist[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Histogram function execution time [ns]:" << time6 << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;

	string fileNamePlacer = isOverall ? "Overall" : monthName[currentMonth - 1];
	WriteFile w = WriteFile();
	w.write_to_txtfile(fileNamePlacer + "_" + std::to_string(inputYear), sortedValues, "Serial");
}

void Parallel(std::vector<float>& Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event)
{
	//Define SerialStatistics object
	SerialStatistics SStats = SerialStatistics();
	//Get the size of the vector
	int Size = Values.size();
	//Sort the vector into ascending
	std::cout << "==================Parallel Run Time=========================" << std::endl;
	std::vector<float> sortedValues = Sort(Values, context, queue, program, prof_event);

	//Display sort kernal execution time
	unsigned long Data = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
	std::cout << "Parallel Sort Kernel execution time [ns]:" << Data << std::endl;
	//Calculate the Sum
	float Sum = SumVec(sortedValues, context, queue, program, prof_event);
	//Display sum kernal execution time
	unsigned long Data2 = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
	std::cout << "Parallel Sum Kernel execution time [ns]:" << Data2 << std::endl;
	//Calculate the mean
	float Mean = Sum / (Size);
	//Calculate the standard deviation
	float SD = STDVec(sortedValues, Mean, context, queue, program, prof_event);
	//Display sort kernal execution time
	unsigned long Data3 = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
	std::cout << "Parallel Standard Deviation Kernel execution time [ns]:" << Data3 << std::endl;
	//Display the total kernel execution time
	auto totalTime = Data + Data2 + Data3;
	parallelTotalTime += totalTime;
	std::cout << "Parallel Total Kernel execution time [ns]:" << totalTime << std::endl << std::endl;

	//Calculate the Min
	float MIN = sortedValues[0];

	//Calculate the Max
	float MAX = sortedValues[Size - 1];
	//Calculate the Median
	float Median = SStats.GetMedianValue(sortedValues);


	//Display header and then all the calculated values
	std::cout << "----------------PARALLEL RESULTS----------------" << std::endl;
	std::cout << "The Sum is: " << Sum << std::endl;
	std::cout << "The Mean is: " << Mean << std::endl;
	std::cout << "The Standard Deviation is: " << SD << std::endl;
	std::cout << "The Min is: " << MIN << std::endl;
	std::cout << "The Max is: " << MAX << std::endl;
	std::cout << "The Median Value is: " << Median << std::endl;

	std::vector<float> histogramValue = sortedValues;
	std::cout << "----------------HISTOGRAM RESULTS FREQUENCY PER BIN----------------" << std::endl;
	Histogram(histogramValue, context, queue, program, prof_event, MAX, MIN);
	unsigned long Data4 = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
	std::cout << "Histogram Kernel execution time [ns]:" << Data4 << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;

	string fileNamePlacer = isOverall ? "Overall" : monthName[currentMonth - 1];
	WriteFile w = WriteFile();
	w.write_to_txtfile(fileNamePlacer + "_" + std::to_string(inputYear), sortedValues, "Parallel");
}

//Sum Vector function
myType SumVec(std::vector<myType>& temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event)
{
	//Set local size to 2
	size_t local_size = 2;
	//Add padding to the vector
	int padding_size = AddPadding(temp, local_size, 0.0f);
	//Set kernal to the reduce addition kernal
	cl::Kernel kernel = cl::Kernel(program, "reduce_add_4");

	float Return = KernalExecRet(kernel, temp, local_size, context, queue, true, false, false, 0.0f, 0, prof_event, "Sum Vector");
	//Return value
	return Return;
}

//Standard deviation function
myType STDVec(std::vector<myType>& temp, myType Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event)
{
	//Get the size of the vector
	double Size = temp.size();
	//Set local size to 2
	size_t local_size = 2;
	//Add padding to the vector
	int padding_size = AddPadding(temp, local_size, 0.0f);
	//Set kernal to the reduce standard deviation kernal
	cl::Kernel kernel = cl::Kernel(program, "reduce_STD_4");
	//Set return to the output from kernal execution
	float Return = KernalExecRet(kernel, temp, local_size, context, queue, true, true, true, Mean, padding_size, prof_event, "Standard Deviation");
	//Divide the result by the size
	Return = (Return / (Size));
	//Square root the result
	Return = sqrt(Return);
	//Return the value
	return Return;
}

std::vector<myType> Sort(std::vector<myType>& temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event)
{
	//Set local size to 32
	size_t local_size = (32);
	//Add padding to the vector
	int padding_size = AddPadding(temp, local_size, -1000000.0f);
	//Set kernel to the parallel selection kernel
	cl::Kernel kernel = cl::Kernel(program, "ParallelSelection");
	//Perform the kernel
	//KernelExec(kernel, temp, local_size, context, queue, false, false, false, 0.0f, 0, prof_event, "Parallel Selection Sort");
	std::vector<myType> sorted = call_kernel(kernel, temp, local_size, context, queue, false, false, false, 0.0f, 0, prof_event, "Parallel Selection Sort");
	//Erase the padded elements at the start of the vector
	sorted.erase(sorted.begin(), sorted.begin() + (local_size - padding_size));
	return sorted;
}

//Function to add padding to an array
int AddPadding(std::vector<myType>& temp, size_t LocalSize, float PadVal)
{
	//Set the local size
	size_t local_size = LocalSize;
	//Find the padding size
	int padding_size = temp.size() % local_size;
	//If there is padding size then
	if (padding_size) 
	{
		//Create an extra vector with PadVal values
		std::vector<float> A_ext(local_size - padding_size, PadVal);
		//Append that extra vector to the input
		temp.insert(temp.end(), A_ext.begin(), A_ext.end());
	}
	//Return padding_size
	return padding_size;
}

//KernalExec is for kernals where the temp vector needs to be overitten
void KernalExec(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name)
{
	//Get the size of the vector
	double Size = temp.size();

	//Get the number of input elements
	size_t input_elements = temp.size();
	//Size in bytes of the input vector
	size_t input_size = temp.size() * sizeof(myType);

	//Define Output vector B
	std::vector<myType> B(input_elements);
	//Get the size in bytes of the output vector
	size_t output_size = B.size() * sizeof(myType);

	//Setup device buffer
	cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

	
	//Write all the values from temp into the buffer
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);

	//Display kernal memory write time
	std::cout << Name << " Kernel memory write time [ns]:" << prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event, ProfilingResolution::PROF_US) << std::endl << std::endl;

	queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

	//Set the arguments 0 and 1 to be the buffers
	kernel.setArg(0, buffer_A);
	kernel.setArg(1, buffer_B);

	//If two is true then set argument two to the local memory size
	if(Two == true)
		kernel.setArg(2, cl::Local(Local_Size * sizeof(myType)));//Local memory size
	//If three is true then set argument three to the float value passed into the function
	if(Three == true)
		kernel.setArg(3, FThree);
	//If four is true then set argument three to the int value passed into the function
	if(Four == true)
		kernel.setArg(4, IFour);

	//Run the kernal
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

	//Copy the result from device to host
	//Setup prof Event
	cl::Event prof_event2;
	queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &temp[0], NULL, &prof_event2);

	//Display kernal memory read time
	std::cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << std::endl << std::endl;
}

//KernalExecRet is for kernals where the temp vector does not need to be overitten, but the first element of B returned
float KernalExecRet(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name)
{
	//Get the size of the vector
	double Size = temp.size();

	//Get the number of input elements
	size_t input_elements = temp.size();
	//Size in bytes of the input vector
	size_t input_size = temp.size() * sizeof(myType);

	//Define Output vector B
	std::vector<myType> B(input_elements);
	//Get the size in bytes of the output vector
	size_t output_size = B.size() * sizeof(myType);

	//Setup device buffer
	cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

	//Set the arguments 0 and 1 to be the buffers
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);

	//Display kernal memory write time
	std::cout << Name << " Kernel memory write time [ns]:" << prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event, ProfilingResolution::PROF_US) << std::endl << std::endl;

	queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

	//Set the arguments 0 and 1 to be the buffers
	kernel.setArg(0, buffer_A);
	kernel.setArg(1, buffer_B);

	//If two is true then set argument two to the local memory size
	if (Two == true)
		kernel.setArg(2, cl::Local(Local_Size * sizeof(myType)));//Local memory size
    //If three is true then set argument three to the float value passed into the function
	if (Three == true)
		kernel.setArg(3, FThree);
	//If four is true then set argument three to the int value passed into the function
	if (Four == true)
		kernel.setArg(4, IFour);

	//Run the kernal
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);
	
	//Copy the result from device to host
	//Setup prof Event
	cl::Event prof_event2;
	queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0], NULL, &prof_event2);

	//Display kernal memory read time
	std::cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << std::endl << std::endl;

	//Return the first element of the buffer Vector B
	return B[0];
}

//DUPE OF KernelExecRet
std::vector<myType> call_kernel(cl::Kernel kernel, std::vector<myType>& temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event& prof_event, std::string Name)
{
	//Get the size of the vector
	double Size = temp.size();

	//Get the number of input elements
	size_t input_elements = temp.size();
	//Size in bytes of the input vector
	size_t input_size = temp.size() * sizeof(myType);

	//Define Output vector B
	std::vector<myType> B(input_elements);
	//Get the size in bytes of the output vector
	size_t output_size = B.size() * sizeof(myType);

	//Setup device buffer
	cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

	//Set the arguments 0 and 1 to be the buffers
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);

	//Display kernel memory write time
	std::cout << Name << " Kernel memory write time [ns]:" << prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event, ProfilingResolution::PROF_US) << std::endl << std::endl;

	queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

	//Set the arguments 0 and 1 to be the buffers
	kernel.setArg(0, buffer_A);
	kernel.setArg(1, buffer_B);

	//If two is true then set argument two to the local memory size
	if (Two == true)
		kernel.setArg(2, cl::Local(Local_Size * sizeof(myType)));//Local memory size
	//If three is true then set argument three to the float value passed into the function
	if (Three == true)
		kernel.setArg(3, FThree);
	//If four is true then set argument three to the int value passed into the function
	if (Four == true)
		kernel.setArg(4, IFour);

	//Run the kernel
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

	//Copy the result from device to host
	//Setup prof Event
	cl::Event prof_event2;
	queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0], NULL, &prof_event2);

	//Display kernel memory read time
	std::cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	std::cout << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << std::endl << std::endl;
	//system("pause");

	return B;
}

// Histogram implementation
void Histogram(std::vector<float>& temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event& prof_event, float maximum, float minimum) {
	cl::Kernel kernel(program, "hist_simple");

	//the following part adjusts the length of the input vector so it can be run for a specific workgroup size
	//if the total input length is divisible by the workgroup size
	//this makes the code more efficient
	size_t local_size = 1024; //1024; //work group size - higher work group size can reduce 
	local_size = 256; //MAXIMUM for Intel UHD Graphics 630
	local_size = 1;
	size_t padding_size = temperature.size() % local_size;

	float paddingVal = 10000;

	//if the input vector is not a multiple of the local_size
	//insert additional neutral elements (0 for addition) so that the total will not be affected (make work for my working set of data)
	if (padding_size) {
		//create an extra vector with neutral values
		std::vector<int> temperature_ext(local_size - padding_size, paddingVal);
		//append that extra vector to our input
		temperature.insert(temperature.end(), temperature_ext.begin(), temperature_ext.end());
	}

	size_t vector_elements = temperature.size();//number of elements
	size_t vector_size = temperature.size() * sizeof(int);//size in bytes

	//Create output vector
	int bin_no = 5;
	vector<int> historgram_vector(bin_no); // histogram results
	vector<int> output(historgram_vector.size());
	size_t output_size = output.size() * sizeof(float);

	//Create buffers
	cl::Buffer input_buffer(context, CL_MEM_READ_WRITE, vector_size);
	cl::Buffer output_buffer(context, CL_MEM_READ_WRITE, output_size);

	//Create queue and copy vectors to device memory
	queue.enqueueWriteBuffer(input_buffer, CL_TRUE, 0, vector_size, &temperature[0]);
	queue.enqueueFillBuffer(output_buffer, 0, 0, output_size);//zero B buffer on device memory

	//Set the arguments 0 and 3 to be the buffers
	kernel.setArg(0, input_buffer);
	kernel.setArg(1, output_buffer);
	kernel.setArg(2, bin_no);
	kernel.setArg(3, minimum);
	kernel.setArg(4, maximum);

	//Execute kernel
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vector_elements), cl::NDRange(local_size));

	//Copy the result from device to host
	queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output_size, &historgram_vector[0]);

	//Remove padding
	temperature.erase(temperature.end() - (local_size - padding_size), temperature.end());

	//display bins and frequency
	std::cout << "Minimum: " << minimum << ", Maximum: " << maximum << std::endl;
	std::cout << "Number of Bins: " << bin_no << ", Bin Size: " << (maximum - minimum) / bin_no << std::endl;
	float binSize = (maximum - minimum) / bin_no;
	for (int i = 1; i < bin_no + 1; i++) {
		float binStart = minimum + (i - 1) * binSize;
		float binEnd = minimum + i * binSize;
		std::cout << "Bin Range: " << binStart << " to " << binEnd << ", Frequency: " << (historgram_vector[i - 1]) << std::endl;
	}
	std::cout << std::endl;
}

// Save the year of Months
void saveYearAndMonths(std::vector<int> uniqueMonth) {
	WriteFile w = WriteFile();
	std::vector<std::string> month;

	for (int i = 0; i < uniqueMonth.size(); i++) {
		month.push_back(monthName[uniqueMonth[i] - 1]);
	}

	w.write_to_txtfile("year", std::to_string(inputYear));
	w.write_to_txtfile("month_list", month);
}