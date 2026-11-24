#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void Skip(ifstream &myFile) {
	myFile>>ws;
	char value=(char)myFile.peek();
	string trash;
	while(value=='#') {
		getline(myFile, trash);
		myFile>>ws;
		value=(char)myFile.peek();
	}
}

int main() {
	int counter(0);
	ifstream myFile("test_missing_pixels.pgm");

	if(!myFile.is_open()) {
		cout<<"Could not open file";
		return 1;

	}

	string type;
	string ramp="@%#*+=-:. ";
	int width,height,maxValue;
	myFile >> type;
	if(type!="P2") {
		cout<<"Wrong type - please make sure image is P2";
		return 1;
	}
	Skip(myFile);
	myFile>>width;
	Skip(myFile);
	myFile>>height;
	Skip(myFile);
	myFile>>maxValue;
	if(myFile.fail()||width<=0 ||height<=0 || maxValue<=0) {
		cout<<"Invalid header in PGM file";
		return 1;

	}


	vector<vector<int>> grid(height, vector<int>(width));

	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			myFile>>grid[i][j];
			if(myFile.fail()) {
				cout<<"there is a problem on row "<<i+1<<" and column "<<j+1;
				return 1;
			}
			if(grid[i][j]<0) {
				counter++;
				grid[i][j]=0;
			}
			else if(grid[i][j]>maxValue) {
				
				counter++;
				grid[i][j]=maxValue;
			}

		}
	}
if(counter>0) {	cout<<"Warning: " << counter << " pixel(s) were out of range and clamped."<<endl;}


	for(int i=0; i<height; i+=2) {
		for(int j=0; j<width; j++) {
			cout<<ramp[grid[i][j]*ramp.size()/(maxValue+1)];

		}
		cout<<endl;
	}
}
