#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include "opencv\cv.h"
#include "opencv2\highgui\highgui.hpp"

using namespace std;
using namespace cv;

vector<Point2f> in1;//vector comprising point co-ordinates from the first convex hull
vector<Point2f> in2;//vector comprising point co-ordinates from the second convex hull
vector<pair<Point2f,Point2f> > pair1;//vector pair containing pairs of point matches from the hulls
vector<pair<float,int> > ang1;//The angle information of points on the first convex hull stored in this vector
vector<pair<float,int> > ang2;//The angle information of points on the second convex hull stored in this vector
pair<Point2f,Point2f> cent(Point(0,0),Point(0,0));//Pair which contains the center of the two convex hulls

//ReadVect reads the pair of Convex Hull Points if no file name is entered as an argument
vector<pair<Point2f,Point2f> > ReadVect()
{
float x1,y1,x2,y2;
int N,i=0;
cin>>N;
vector<pair<Point2f,Point2f> > inp;
while(N--)
    {
    cin>>x1>>y1>>x2>>y2;
    in1.push_back(Point2f(x1,y1));
    in2.push_back(Point2f(x2,y2));
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
float sx1=0,sx2=0,sy1=0,sy2=0;
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
cout<<"Canter of first convex hull : "<<cent.first<<endl<<"Center of second convex hull : "<<cent.second<<endl;
}

//Function to evaluate the angle of the points on the hull relative to the center
void EvalAngle(vector<pair<Point2f,Point2f> > In)
{
float t1,t2,ang;
int pno=0;
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

//Function to Display a vector of pairs<float,int>
void DisplaySort(vector<pair<float,int> > a)
{
vector<pair<float,int> >::iterator it;
for(it=a.begin();it!=a.end();++it)
        {
        pair<float,int> tm=*it;
        cout<<"Angle:"<<tm.first<<" Index:"<<tm.second<<endl;
        }
}

//Function to sort the points on the basis of their angle with the x-axis
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

//Function to extract the sequence of the points from the vector<pair<float,int> > and push the result in a vector of strings
void ExtrSeq(vector<pair<float,int> > in,vector<string> *res)
{

    //cout<<(int)in.size()<<endl;
    char tmp[2];
    vector<string> t;
    vector<pair<float,int> >::iterator it;
    for(it=in.begin();it!=in.end();it++)
        {
        pair<float,int> tm=*it;
        itoa(tm.second,tmp,10);
        t.push_back(tmp);
        }
    *res=t;
    //cout<<res<<endl;
}

//Utility function to get the maximum of two ints
int max(int a, int b)
{
    return (a > b)? a : b;
}

//Function to find the longest common sequence to get the no. of points in the same rotational order
int RotOrder(vector<string> X,vector<string> Y )
{
    int m=X.size(),n=Y.size(),i,j;
    vector<string>::iterator it1,it2;
    int L[m][n];
    for (i=0,it1=X.begin();i<m; i++,it1++)
        {
        for (j=0,it2=Y.begin();j<n; j++,it2++)
            {
            if((*it1).compare(*it2)==0)
                L[i][j]=(i==0||j==0)?1:L[i-1][j-1]+1;
            else
                L[i][j]=(i==0||j==0)?0:max(L[i-1][j],L[i][j-1]);
            //printf("%d",L[i][j]);
            }
        //printf("\n");
        }
    return L[m-1][n-1];
}

float StdDev(vector<pair<float,int> > an1,vector<pair<float,int> > an2)
{
    float a,a2=0,ans=0;
    vector<pair<float,int> >::iterator it1,it2;
    it1=an1.begin();
    int cnt=0;
    pair<float,int> init1=*it1;
    for(it1=an1.begin();it1!=an1.end();it1++)
        {
        //cout<<bool(it1==an1.end()-1)<<endl;
        pair<float,int> t1=*it1,t2=(it1==an1.end()-1)?init1:*(it1+1),s1;
        a=t1.first-t2.first;
        //cout<<"Angle::"<<a<<" Points::"<<t1.second<<","<<t2.second<<endl;
        cnt=0;a2=0;
        for(it2=an2.begin();it2!=an2.end();it2++)
            {
            s1=*it2;

            if(cnt==2)
                break;
            if(s1.second==t1.second)
                {
                a2+=s1.first;
                cnt++;
                }
            else if(s1.second==t2.second)
                {
                a2-=s1.first;
                cnt++;
                }
            //cout<<"Loop:"<<s1.first<<" "<<s1.second<<":"<<a2<<endl;
            }
        //cout<<"Angle2::"<<a2<<" diff::"<<a-a2<<endl;
        ans+=pow(min(((a-a2)<0?-(a-a2):(a-a2)),360-((a-a2)<0?-(a-a2):(a-a2))),2);
        //cout<<"Dev::"<<ans<<endl;
        }
        ans=sqrt(ans)/an1.size();
    //cout<<ans<<endl;
    return ans;
}
//Main
int main(int argc, char *argv[])
{
    float pars[4];
    string in;
    int cn,i=0;
    if(argc!=2)
        {
        //If no arguments the data is read from the user using ReadVect() function
        pair1=ReadVect();
        DisplayVect(pair1);
        }
    else
        {
        //Reading data from a file in the format float,float:float,float (for ex. 45.66,23.12:12.33,56.75)
        ifstream f(argv[1]);//Command line input for file name
        if(f.is_open())
            while(f.good())
                {
                cn=0;
                getline(f,in);
                //cout<<in<<endl;
                char delims[]=" ,:",*res,*cstr;
                cstr=new char[in.size()+1];
                strcpy(cstr,in.c_str());
                res=strtok(cstr,delims);
                while (res != NULL)
                    {
                    pars[cn++]=atof(res);
                    res = strtok(NULL, delims);
                    }
                in1.push_back(Point2f(pars[0],pars[1]));
                in2.push_back(Point2f(pars[2],pars[3]));
                pair<Point2f,Point2f> a(in1[i],in2[i]);
                //cout<<"Pair::"<<a.first<<a.second<<endl;
                pair1.push_back(a);
                i++;
                }

        }
    EvalCenter(pair1);

    EvalAngle(pair1);

    SortAngle(&ang1,&ang2);

    //DisplaySort(ang1);
    //DisplaySort(ang2);

    vector<string> s1;
    ExtrSeq(ang1,&s1);
    cout<<"Rotational sequence of points in first convex hull :"<<endl;
    for(vector<string>::iterator it=s1.begin();it!=s1.end();it++)
        cout<<*it<<" ";
    cout<<endl;

    int len=s1.size();
    for(i=0;i<len/2;i++)
        s1.push_back(s1.at(i));

    vector<string> s2;
    ExtrSeq(ang2,&s2);
    cout<<"Rotational sequence of points in second convex hull :"<<endl;
    for(vector<string>::iterator it=s2.begin();it!=s2.end();it++)
        cout<<*it<<" ";
    cout<<endl;

    int InRot=RotOrder(s1,s2);
    cout<<"No of points not in rotational order :: "<<len-InRot<<endl;

    float ans=StdDev(ang1,ang2);
    cout<<"Standard Deviation::"<<ans<<endl;
    return 0;
}
