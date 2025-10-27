# ArduinoNBT Library
### A library for arduino to work with NBT data

### **⚠️ The project is in active development**

### Installing
1) Download the project
2) Put **nbt.h** and **nbt.cpp** into your project folder

### Usage:

Include the library (the path may be different if you place the library outside the root of your project):
```cpp
#include "nbt.h"
```

Creating an NBT value:
```cpp
short yourShort = 1;
NBTShort nbtShort("Name of short in NBT format", yourShort);
```

Converting NBT to a byte array (for example, to send it in packets)
```cpp
// Creating byte array
size_t sizeOfBuffer = nbtShort.getBufferSize();
byte byteBuffer[sizeOfBuffer];

// Writing nbt into a byte array
size_t writtenBytes = nbtShort.serialize(byteBuffer);

// writtenBytes is amount of written bytes
```

Convert a byte array to an NBT Byte Array:
```cpp
// Your byte array
byte array[5] = {
  0x01,
  0x02,
  0x03,
  0x04,
  0x05
};

// Create a buffer
byte buffer[50];

// Write your byte array to the buffer, converting it to an NBT Byte Array
size_t amount = serializeByteArray(buffer, "My NBT Byte Array", array, sizeof(array));
// "amount" is amount of written bytes
```

Using NBTString:
```cpp
NBTString nbtString("My NBT String", "Some text for test converting text to an NBT String");

byte buffer[100];

size_t size = nbtString.serialize(buffer);
```

**Using serializeNBTNumberList:**

Please use the types with the specified size
- short: int16_t
- int: int32_t
- long: int64_t

And standart ones:
- float: float
- double: double

You also need to specify a tag, all possible tags are listed below
```cpp
int32_t array[] = {
  1,
  2,
  3,
  4,
  5
};

uint16_t size_of_array = 5;
const char* name_of_nbt_array = "My int NBT List";
byte buffer[200];

size_t size = serializeNBTNumberList<int32_t>(buffer, name_of_nbt_array, array, size_of_array, ID_TAG_INT);
```

**Using serializeNBTStringList:**
```cpp
const char* array[] = {
  "cat",
  "wolf",
  "parrot",
  "fox"
};

uint16_t size_of_array = 4;
const char* name_of_nbt_array = "My string NBT List";
byte buffer[200];

size_t size = serializeNBTStringList(buffer, name_of_nbt_array, array, size_of_array);
```

**Using Compound Tags:**
```cpp
// Create Compound Tag
// Note: The second parameter is the size of the buffer in the class, so it must be specified in bytes.
NBTCompoundTag compound("My Compound Tag", 100);

// Adding int (all methods can be seen below)
// Note: If the serialized tag size is larger than the buffer size in the class (which you specified earlier), the value will not be added, without any warning
compound.pushInt("My Int", 777);

// Serializing Compound Tag
byte buffer[100];
size_t size = compound.serialize(buffer);
```
Available methods in Compound Tag:
- pushByte
- pushShort
- pushInt
- pushLong
- pushFloat
- pushDouble

And if you've already serialized the nbt tag, you can use `pushBytes(bytesArray)` to add it to the compound tag.

**Available types:**
- NBTShort
- NBTInt
- NBTLong
- NBTFloat
- NBTDouble
- NBTString
- NBTCompoundTag

**Available functions:**
- serializeByteArray (Converts your byte array to an NBT Byte Array)
- serializeNBTNumberList (Create NBT List of any numbers (short, int, long, float, double))
- serializeNBTStringList (Create NBT List of string)

**Available TAGS:**
- ID_TAG_END (0x00)
- ID_TAG_BYTE (0x01)
- ID_TAG_SHORT (0x02)
- ID_TAG_INT (0x03)
- ID_TAG_LONG (0x04)
- ID_TAG_FLOAT (0x05)
- ID_TAG_DOUBLE (0x06)
- ID_TAG_BYTE_ARRAY (0x07)
- ID_TAG_STRING (0x08)
- ID_TAG_LIST (0x09)
- ID_TAG_COMPOUND (0x0A)
- ID_TAG_INT_ARRAY (0x0B)
- ID_TAG_LONG_ARRAY (0x0C)
