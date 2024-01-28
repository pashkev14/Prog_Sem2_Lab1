#pragma once

#include "files.h"
#include "word.h"

void solve(Word* words, int size, char key, Files* files);

void readInStopMode(Files* files);

void readInLimitMode(Files* files);

void process(Files* file);