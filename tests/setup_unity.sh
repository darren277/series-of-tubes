#!/bin/bash

# Create unity directory if it doesn't exist
mkdir -p unity

# Download Unity test framework
curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.c -o unity/unity.c
curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.h -o unity/unity.h
curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity_internals.h -o unity/unity_internals.h

echo "Unity test framework downloaded successfully!" 