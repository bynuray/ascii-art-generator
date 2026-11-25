#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>

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
int Start(int j, double scale) {
	double res=j*scale;
	return res;
}

int main( int argc, char *argv[]) {
	if(argc<2 ||argc>3) {
		cout << "Usage: ./ascii <file.pgm> [width]" << endl;
		cout << "Example: ./ascii starry_night.pgm 120" << endl;
		return 1;
	}

	int counter(0);
	ifstream myFile(argv[1]);

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
	if(counter>0) {
		cout<<"Warning: " << counter << " pixel(s) were out of range and clamped."<<endl;
	}

	int outputWidth(80);
	if(argc>2) {
	string w=argv[2];
	
	for(int i=0; i<w.size();i++){
	    if(!isdigit(w[i])){
	        cout<<"Width must be a positive whole number, e.g. 80"<<endl;
	        return 1;
	    }
	}
	try{
	    
		outputWidth=stoi(argv[2]);
	
	}
	catch(...){
	    cout<<"Please enter number"<<endl;
	    return 1;
	}    
	}
	
	
	
    if(outputWidth<1) {
		cout<<"Width must be a positive number (e.g. 80)."<<endl;
		return 1;
	}
	if(outputWidth>width) {
		outputWidth=width;
	}
	double scale=(double)width/(double)outputWidth;
	int outputHeight = height / scale / 2;
	if(outputHeight<1) {
		outputHeight=1;
	}
	double row_scale=(double)height/(double)outputHeight;

cout<<outputHeight<<" "<<outputWidth<<endl;
	for(int i=0; i<outputHeight; i++) {
		int first_row=Start(i,row_scale);
		int last_row=Start(i+1,row_scale)-1;
		for(int j=0; j<outputWidth; j++) {
			int sum(0),count(0);
			int first=Start(j,scale);
			int last=Start(j+1,scale)-1;

			for (int y = first_row; y<=last_row; y++) {
				for (int x = first; x<=last; x++) {
					sum += grid[y][x];
					count++;
				}

			}

			int avg=sum/count;

			cout<<ramp[avg*ramp.size()/(maxValue+1)];

		}
		cout<<endl;
	}
}
