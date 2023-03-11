#pragma once
#include<winsock2.h>
#include<stdio.h>
#include<stdlib.h>

void logerror(const char* seq) {
    printf("there is an error happen, which is %s function\n", seq);
    exit(-1);
}
