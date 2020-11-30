/**
* CS 3345 HON
* Project 2, HASH TABLE
* Used compiler g++ 4.8.5 - 11 with compile command : g++ -std=c++11 - o hash HashTable.cpp
* @author CMLQ
* date 10 / 27 / 20
*/

#include <iostream>
#include <fstream>
#include <string>
// Entry class
class Entry 
{
	public:
		std::string Key; //from 1-20 upper-case alphabetic chars
		std::string Value; //from 1-20 chars
		int avail; //unused = 0, free = 1, occupied = 2
		//constructor for entry
		Entry(std::string Key, std::string Value, int avail);
};
// constructor for entry, sets the Key, Value and avial
Entry::Entry(std::string Key, std::string Value, int avail)
{
	this->Key = Key;
	this->Value = Value;
	this->avail = avail;
}
// MyHashTable Class
class MyHashTable
{
	public:
		// the table itself
		Entry** table;
		// the table size;
		int size;
		// the prime number associated with the table;
		int prime;
		// number of probes for unsuccessful searches
		int unsuccessfulSearchProbes;
		// number of probes for successful searches
		int successfulSearchProbes;
		// number of probes for successful inserts
		int successfulInsertProbes;
		//constructur that takes the table size and prime
		MyHashTable(int, int);
		// insert an entry based off a key and string, returns true if successfully inserted, false if otherwise
		bool insert(std::string, std::string);
		// find an entry based off a key, returns the value of the key if present, an empty string otherwise
		std::string find(std::string);
		// remove an entry based off a key, returns true if successfully removed, false if otherwise
		bool remove(std::string);
		// lists the amount of nonempty entries in the table
		int membership();
		// lists all entries in key value pairs along with position
		void listAll();
		//uses horner method to simplify the hash function
		unsigned int horner_hash(std::string, int);
		//first hash function
		int hash(std::string, int);
		// second hash function
		int hash2(std::string, int);
		

		~MyHashTable();


};
//empties the table
MyHashTable::~MyHashTable()
{
	for (int i = 0; i < size; i++)
	{

		table[i]->Value = "";
		table[i]->avail = 0;
		table[i]->Key = "";

	}
}
//function that simplifies the hash function with Horners method
unsigned int MyHashTable::horner_hash(std::string key, int x)
{
	
	unsigned int result = key[0];

	for (unsigned int i = 1; i < key.length(); i++)
	{
		result = (unsigned int)(result * x + key[i]);
	}

	return result;
}
// hash function, using the horner_hash function
int MyHashTable::hash(std::string key, int tablesize)
{
	return horner_hash(key, 31) % tablesize;
}
// second hash function
int MyHashTable::hash2(std::string key, int tablesize)
{
	int x = prime - (hash(key, tablesize) % prime);
	return x;
}
// creates an empty hash table based off a size and prime number
MyHashTable::MyHashTable(int sz, int R)
{
	this->size = sz;
	this->prime = R;
	table = new Entry * [sz];
	for (int i = 0; i < sz; i++)
	{

		table[i] = new Entry("", "", 0);


	}
	unsuccessfulSearchProbes = 0;
	successfulSearchProbes = 0;
	successfulInsertProbes = 0;

}
// insert an entry based off a key and string, returns true if successfully inserted, false if otherwise
bool MyHashTable::insert(std::string key, std::string value)
{
	bool inserted = false;
	int originalSSearchProbe = successfulSearchProbes;
	int originalUSearchProbe = unsuccessfulSearchProbes;
	std::string found = find(key);
	successfulSearchProbes = originalSSearchProbe;
	unsuccessfulSearchProbes = originalUSearchProbe;
	if (found != "")
	{
		return inserted;
	}
	int index1 = hash(key, size);
	int index2 = hash2(key, size);
	int i = 0;
	int index3 = index1;
	while (table[index3]->avail != 0 || table[index3]->avail != 1)
	{
		index3 = (index1 + i * index2);
		// if index3 becomes bigger than the size of the table, wrap the index to the front (beginning) of the table
		while (index3 >= size)
		{
			index3 = index3 - size;
		}
		successfulInsertProbes++;
		if ((table[index3]->avail == 0 || table[index3]->avail == 1))
		{
			table[index3]->Value = value;
			table[index3]->Key = key;
			table[index3]->avail = 2;

			inserted = true;
			break;
		}
		i++;
	}
	return inserted;
}
// find an entry based off a key, returns the value of the key if present, an empty string otherwise
std::string MyHashTable::find(std::string key)
{
	std::string found = "";

	int index1 = hash(key, size);
	int index2 = hash2(key, size);
	int i = 0;
	int numProbes = 1;
	//index 3 will serve as our probing sequence
	int index3 = index1;

	while (table[index3]->avail != 0 && table[index3]->Key != key)
	{
		i++;
		numProbes++;
		//probing function
		index3 = index1 + i * index2;

		// if index becomes bigger than the size of the table, wrap the index to the front (beginning) of the table
		while (index3 >= size)
		{
			index3 = index3 - size;
		}
	}
	found = table[index3]->Value;

	// an unsuccessful search, increase the number of unsuccessfulSearchProbes
	if (found == "")
	{
		unsuccessfulSearchProbes += numProbes;
	}
	else // a succesful search, increase the number of successfulSearchProbes
	{
		successfulSearchProbes += numProbes;
	}
	
	return found;	
}
// remove an entry based off a key, returns true if successfully removed, false if otherwise
bool MyHashTable::remove(std::string key)
{
	bool removed = false;
	int index1 = hash(key, size);
	int index2 = hash2(key, size);
	//index 3 will serve as a probing sequence
	int index3 = index1;
	int i = 0;

	//while the avail at updated index is not equal to empty
	while (table[index3]->avail != 0)
	{

		index3 = (index1 + i * index2);
		// if index3 becomes bigger than the size of the table, wrap the index to the front (beginning) of the table
		while (index3 >= size)
		{
			index3 = index3 - size;
		}

		//key has been found, make it empty
		if (table[(index3)]->avail == 2 && table[(index3)]->Key == key) {
			table[index3]->avail = 1;
			table[index3]->Value = "";
			table[index3]->Key = "";

			removed = true;
			break;
		}
		i++;
	}

		return removed;
}
//outputs number of nonempty members
int MyHashTable::membership()
{
	int memcount = 0;
	for (int i = 0; i < size; i++)
	{
		if (table[i]->avail == 2)
		{
			memcount++;
		}
	}
	return memcount;
}
// outputs all nonempty entries with its position, key and value
void MyHashTable::listAll()
{
	for (int i = 0; i < size; i++)
	{
		if (table[i]->avail == 2)
			std::cout << i << " " << table[i]->Key << ":" << table[i]->Value << std::endl;

	}
}
int main()
{

	// file to read
	std::ifstream myfile;
	myfile.open("HashTableText.txt");

	//variables necessary for commands and inputs
		std::string key, newKey, delimiter = ":", val;
		bool insert, remove;
		char command, test;
		int originalSSearchProbe, originalUSearchProbe;
		//table size, prime number
		int sz, R;
		//successful insert count, successful search count, unsuccessful search count
		int si = 0, ss = 0,us = 0, delim = 0;
		
	std::string find;
	MyHashTable ta(0, 0);

	while (myfile >> command)
	{
		switch (command)
		{
		case 'D':
			myfile >> sz >> R;
			(ta) = MyHashTable(sz, R);

			break;
		case 'C':
			//clear
			ta.~MyHashTable();
			si = 0;
			ss = 0;
			us = 0;
			ta.successfulInsertProbes = 0;
			ta.successfulSearchProbes = 0;
			ta.unsuccessfulSearchProbes = 0;
			
			break;
		case 'H':
			myfile >> key;
			std::cout << key << " " << ta.hash(key, 17) << std::endl;
			break;

		case 'A':
			
			//insert
			myfile >> key;
			newKey = key.substr(0, key.find(delimiter));
			val = key.substr(key.find(delimiter) + 1, key.size() - 1);
			
			insert = (ta).insert(newKey, val);
			if (insert == true)
			{
				std::cout << "Key " << newKey << " inserted" << std::endl;
				si++;
			}
			else
			{
				std::cout << "Key " << newKey << " already exists" << std::endl;
			}
			break;
		case 'R':

			//delete
			myfile >> key;
			remove = (ta).remove(key);
			if (remove == true)
			{
				std::cout << "Key " << key << " deleted" << std::endl;
			}
			else
			{
				std::cout << "Key " << key << " not found" << std::endl;
			}
			break;
		case 'S':

			//search
			myfile >> key;
			find = (ta).find(key);
			if (find == "")
			{
				std::cout << "Key " << key << " not found" << std::endl;
				us++;
			}
			else
			{
				std::cout << "Key " << key << ":" << find << std::endl;
				ss++;
			}
			break;
		case 'M':

			std::cout << "Membership is " << (ta).membership() << "\n";
			break;
		case 'P':

			ta.listAll();
			break;
		case 'T':

			std::cout << "Total Number of Successful Inserts = " << si << std::endl;
			std::cout << "Total Number of Probes on Successful Inserts = " << ta.successfulInsertProbes << std::endl;
			std::cout << "Total Number of Successful Searches = " << ss << std::endl;
			std::cout << "Total Number of Probes on Successful Searches = " << ta.successfulSearchProbes << std::endl;
			std::cout << "Total Number of Unsuccessful Searches = " << us << std::endl;
			std::cout << "Total Number of Probes on Unsuccessful Searches = " << ta.unsuccessfulSearchProbes << std::endl;
			break;
		case 'E':
			exit(0);

		}

	}

	myfile.close();

	return 0;

}

