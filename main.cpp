#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "opencv\cv.h"
#include "opencv2\highgui\highgui.hpp"

using namespace std;
using namespace cv;

vector<Point2f> in1;
vector<Point2f> in2;
vector<pair<Point2f,Point2f> > pair1;
vector<pair<float,int> > ang1;
vector<pair<float,int> > ang2;
pair<Point2f,Point2f> cent(Point(0,0),Point(0,0));

//ReadVect reads the pair of Convex Hull Points if no file name is entered as an argument
vector<pair<Point2f,Point2f> > ReadVect()
{
int x1,y1,x2,y2;
int N,i=0;
cin>>N;
vector<pair<Point2f,Point2f> > inp;
while(N--)
    {
    cin>>x1>>y1>>x2>>y2;
    in1.push_back(Point(x1,y1));
    in2.push_back(Point(x2,y2));
    pair<Point2f,Point2f> a(in1[i],in2[i]);
    //cout<<"Pair::"<<a.first<<a.second<<endl;
    inp.push_back(a);
    i++;
    }
return inp;
}

//Function to display the vector pair
void DisplayVect(vector<pair<Point2f,Point2f> > Prin)
{
vector< pair<Point2f,Point2f> >::iterator it;
for(it=Prin.begin();it!=Prin.end();it++)
    {
    pair<Point2f,Point2f> x=*it;
    cout<<x.first<<x.second<<endl;
    }

}

//Function to evaluate the center of the two hulls
void EvalCenter(vector<pair<Point2f,Point2f> > In)
{
int sx1=0,sx2=0,sy1=0,sy2=0;
int cnt=0;
vector< pair<Point2f,Point2f> >::iterator it;
for(it=In.begin();it!=In.end();it++)
    {
    pair<Point2f,Point2f> x=*it;
    sx1+=x.first.x;
    sy1+=x.first.y;
    sx2+=x.second.x;
    sy2+=x.second.y;
    cnt++;
    }
//cout<<sx1<<sy1<<sx2<<sy2<<endl;
cent.first.x=sx1/cnt;
cent.first.y=sy1/cnt;
cent.second.x=sx2/cnt;
cent.second.y=sy2/cnt;
cout<<cent.first<<cent.second<<endl;
}

//Function to evaluate the angle of the points on the hull relative to the center
void EvalAngle(vector<pair<Point2f,Point2f> > In)
{
int t1,t2,pno=0;
float ang;
vector< pair<Point2f,Point2f> >::iterator it;
for(it=In.begin();it!=In.end();it++)
    {pno++;
    pair<Point2f,Point2f> x=*it;
    t1=x.first.x-cent.first.x;
    t2=x.first.y-cent.first.y;
    ang=atan2(t2,t1)*180/M_PI;
    //cout<<"An1::"<<ang<<endl;
    pair<float,int> tmp(ang,pno);
    ang1.push_back(tmp);
    t1=x.second.x-cent.second.x;
    t2=x.second.y-cent.second.y;
    ang=atan2(t2,t1)*180/M_PI;
    tmp.first=ang;tmp.second=pno;
    ang2.push_back(tmp);
    //cout<<"An2::"<<ang<<endl;
    }
}
void DisplaySort(vector<pair<float,int> > a)
{
vector<pair<float,int> >::iterator it;
for(it=a.begin();it!=a.end();++it)
        {
        pair<float,int> tm=*it;
        cout<<"Angle:"<<tm.first<<" Index:"<<tm.second<<endl;
        }
}

void SortAngle(vector<pair<float,int> > *x1,vector<pair<float,int> > *x2)
{
vector<pair<float,int> > a1=*x1;
vector<pair<float,int> > a2=*x2;
sort(a1.begin(),a1.end());
sort(a2.begin(),a2.end());
//DisplaySort(a1);
//DisplaySort(a2);
*x1=a1;*x2=a2;
}

//void RotOrder()
int main(int argc, char *argv[])
{
    //float x1,x2,y1,y2;
    char in[100],p[100];
    if(argc!=2)
        {
        pair1=ReadVect();
        DisplayVect(pair1);
        }
    else
        {
        /*FILE *f=fopen(argv[1],"r");
        while(fscanf(f,"%s",in)!=EOF)
            {

            //cout<<in<<endl;
            //sscanf(in,"%*[%f%*,%f%*]%*,%*[%f%*,%f%*]",&x1,&y1,&x2,&y2);
            //cout<<p<<endl;
            }*/

        }
    EvalCenter(pair1);
    EvalAngle(pair1);
    SortAngle(&ang1,&ang2);
    DisplaySort(ang1);
    DisplaySort(ang2);

    return 0;
}
