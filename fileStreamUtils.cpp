#include "fileStreamUtils.h"



long getFileSize(FILE * file_ptr){
    long file_length;
    if(file_ptr != NULL){
        // Get the file size
        fseek(file_ptr, 0, SEEK_END);
        file_length = ftell(file_ptr);
        rewind(file_ptr);
    } else {
        file_length = -1;
    }
    return file_length;
}

void answerToCheckRequest(int socketFD){
    int file_name_length = receive4Byte(socketFD);  // Receive file name length
    char * name = receiveNByte(socketFD , file_name_length); // Receive file name
    sendFileIsExist(socketFD , name , file_name_length); // Response to it
    delete[] name;
}

void sendFileIsExist(int socketFD, char * file_name , int file_name_length){

    FILE * file_ptr;
    file_ptr = fopen(file_name , BINARY_READER);
    long file_length = getFileSize(file_ptr);  
    send4Byte(socketFD , ANSWER_TO_EXIST_CODE); // Sending ANSWER_TO_EXIST_CODE to socket
    send8Byte(socketFD , file_length); // Sending file length to socket(send -1 if file not exist)

    // Check file is exist to send more data
    if(file_ptr != NULL){
        // Send file name length and file name
        send4Byte(socketFD , file_name_length);
        sendNByte(socketFD , file_name , file_name_length);
    }
}

void responseToFileIsExist(int socketFD){
    long file_length = receive8Byte(socketFD);
    if(file_length != -1){
        int file_name_length = receive4Byte(socketFD);
        char * name = receiveNByte(socketFD , file_name_length);
        printf("received file with size : %ld and name : %s\n",file_length , name);
        delete[] name;
    }
}