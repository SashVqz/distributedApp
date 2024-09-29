/*
	operations.h --- Sash Vazquez
	RPCs for the file manager class and methods for packing and unpacking
*/

#pragma once

typedef enum fileManagerOp {
	opBuilder = 1,
	opDestroyer = 2,
	opListFile = 3, 
	opReadFile = 4,
	opWriteFile = 5
} fileManagerOp;

template <typename T>
void pack(std::vector<unsigned char> &packet, T data) {
	unsigned char* ptr = (unsigned char*)&data;
	int numBytes = sizeof(T);
	int size = packet.size();
	packet.resize(size + numBytes);
	std::copy(ptr, ptr + numBytes, packet.begin() + size);	
}

template <typename T>
void packv(std::vector<unsigned char> &packet, T* data, int size) {
	for (int i = 0; i < size; i++) {
		pack(packet, data[i]);
	}
}

template <typename T>
T unpack(std::vector<unsigned char> &packet) {
	T data;
	T* ptr = (T*)packet.data();
	int numBytes = sizeof(T);
	int size = packet.size();
	
	data = ptr[0];
	for (int i = numBytes; i < size; i++) {
		packet[i - numBytes] = packet[i];
	}
	packet.resize(size - numBytes);

	return data;
}

template <typename T>
void unpackv(std::vector<unsigned char> &packet, T* data, int size) {
	for (int i = 0; i < size; i++) {
		data[i] = unpack<T>(packet);
	}
}
