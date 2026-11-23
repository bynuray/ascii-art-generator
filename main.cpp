#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main() {

	ifstream myFile("starry_night.pgm");

	if(!myFile.is_open()) {
		cout<<"Could not open file";
		return 1;

	}

	string type;
	string ramp="@%#*+=-:. ";
	int width,height,maxValue;
	myFile >> type>>width>>height>>maxValue;

	if(type!="P2") {
		cout<<"Wrong type - please make sure image is P2";
		return 1;
	}

	vector<vector<int>> grid(height, vector<int>(width));

	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			myFile>>grid[i][j];

		}
	}


	for(int i=0; i<height; i+=2) {
		for(int j=0; j<width; j++) {
			cout<<ramp[grid[i][j]*ramp.size()/(maxValue+1)];

		}
		cout<<endl;
	}
}