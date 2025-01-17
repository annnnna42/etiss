/**
 * Generated on Thu, 24 Feb 2022 17:15:20 +0100.
 *
 * This file contains the GDBCore adapter for the RV32IMACFD core architecture.
 *
 * WARNING: This file contains user-added code, be mindful when overwriting this with
 * generated code!
 */

#ifndef ETISS_RV32IMACFDArch_RV32IMACFDGDBCORE_H_
#define ETISS_RV32IMACFDArch_RV32IMACFDGDBCORE_H_

#include "etiss/IntegratedLibrary/gdb/GDBCore.h"
#include <sstream>

/**
	@brief This class is the brige between RV32IMACFD architecture and gdbserver

	@details Gdbserver integrated in ETISS calls GDBCore to read/write registers via virtualStrruct
				The index in mapRegister() should strictly follow the RV32IMACFD gdb tool defined register
				order. Because gdbserver will send raw register data sequentially in strict order over
				RSP ->TCP/IP ->RSP protocal

				Check the order with gdb command:
				$(gdb) info all-registers
				which lists all registers supported and its order.

				By default only general purpose register and instruction pointer are supported. Further
				Special Function Register/Control and Status Register could be added manually. Meanwhile
				virtualStruct in RV32IMACFDArch.cpp should be modified as well as well

*/
class RV32IMACFDGDBCore : public etiss::plugin::gdb::GDBCore {
public:
	std::string mapRegister(unsigned index){
		if (index < 32){
		printf("mapRegister normal 0...31 index: %d \n", index);
			std::stringstream ss;
			ss << "X" << index;
			return ss.str();
		} 
		if ((32 < index) and (index < 65)){
		printf("mapRegister float 33...64 index: %d \n", index);
			std::stringstream ss;
			ss << "F" << (index - 33);
			return ss.str();
		}

		if ((64 < index) and (index < 69)){
		printf("mapRegister CSR float 65...68 index: %d \n", index);
			std::stringstream ss;
			ss << "CSR" << (index - 65);
			return ss.str();
		}

		switch (index){
		case 32:
			return "instructionPointer";
/*		case 65:
			return "CSR0";
		case 66:
			return "CSR1";
		case 67:
			return "CSR2";
		case 68:
			return "CSR3"; 
*/
		/**************************************************************************
		*   Further register should be added here to send data over gdbserver	  *
		***************************************************************************/
		}
		return "";
	}

	unsigned mapRegister(std::string name){
		printf("mapRegister(std::string name\n");
		return INVALIDMAPPING;
	}

	unsigned mappedRegisterCount(){
		printf("mappedRegisterCount()\n");
		// Modify according to sent register number
		return 69;
	}

	etiss::uint64 getInstructionPointer(ETISS_CPU * cpu){
		return cpu->instructionPointer;
	}

	bool isLittleEndian(){
		// Modify according to RV32IMACFD manual
		return true;
	}
};

#endif
