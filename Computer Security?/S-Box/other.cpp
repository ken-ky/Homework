#include<iostream>
#include<string>
#include<sstream>

using namespace std;

string m = "11B";
string f = "";
string end = "01100011";//63进制
//string x[8];//矩阵x
////初始化 X
//void Initialize_x(){
//	x[0]="10001111";
//	x[1]="11000111";
//	x[2]="11100011";
//	x[3]="11110001";
//	x[4]="11111000";
//	x[5]="01111100";
//	x[6]="00111110";
//	x[7]="00011111";
//}

//将16进制的字符串转2进制的字符串 
string Hex_to_B(string num);

//2进制表示转16进制
string B_to_Hex(string a);

//将16进制的字符串转10进制
int Hex_to_Dec(string s);

//将10进制转16进制的字符串
string Dec_to_Hex(int a);

//将10进制转16进制的字符串
string Dec_to_Hex(int a);

//1*8  乘  8*1
char Mul_(string a, string b);

//位异或运算
char Xor(char n1, char n2);

//左移n位
string Left_n(string a, int n);

//两个多项式 异或
string Xor_arr(string a, string b);

//仿射变换
string S_Box(string a);

//gf(2,8)乘法
string Mul(string a, string b) {
    string temp[8];//存储中间结果
    //计算中间结果
    temp[0] = a;
    for (int i = 1; i < 8; i++) {
        temp[i] = Left_n(temp[i - 1], 1);
        if (temp[i - 1][0] == '1')//判断是否还需要异或1B
            temp[i] = Xor_arr(temp[i], "00011011");
    }
    //最后计算
    string res = "00000000";
    //遍历g ，将f和中间结果异或
    for (int i = 0; i < 8; i++) {
        if (b[i] == '1')
            res = Xor_arr(res, temp[7 - i]);
    }
    return res;
}

//gf(2,8)除法
void Div(string a, string b, string &r, string &s) {
    r = b;
    s = "00000000"; //r余数  s商
    bool flag = false;
    if (a == "100011011") { flag = true; }//一开始的11B
    string t = "00000000";
    while (true) {//余式的最高次项 小于 除式的最高次项 结束
        int n = flag ? b.find('1') + 1 : a.find('1') - b.find('1');//被除数的最高次 - 除数的最高次
        n = n > 0 ? n : -n;
        if (flag) {
            flag = false;
            a = a.substr(1, 8);
        }
        r = Left_n(b, n);//左移，同高次
        //cout<<n<<"\t 左移："<<r<<"\t 商：";
        s[7 - n] = '1';
        //cout<<s<<"\t 余数：" ;
        r = Xor_arr(r, a);
        //cout<<r<<endl;
        a = r;
        //system("pause");
        if (b.find('1') < r.find('1')) {
            //cout<<"符合除法，结束"<<endl;
            break;
        }
    }
}


//求乘法逆元 
string Mul_inverse(string s) {
    if (s == "00") return "00000000";//00的 逆元 为0
    if (s == "01") return "00000001";//01的 逆元 为1
    string arr[2][3];
    arr[0][0] = "00000001";
    arr[0][1] = "00000000";
    arr[0][2] = "100011011";
    arr[1][0] = "00000000";
    arr[1][1] = "00000001";
    arr[1][2] = Hex_to_B(s);
    //cout<<"被除数："<<arr[0][2]<<"  除数："<<arr[1][2]<<endl;
    int i = 0;
    string q = "", r = "";//n商， r余数
    while (true) {
        Div(arr[i][2], arr[(i + 1) % 2][2], r, q);
        //cout<<"商："<<q<<"  余数："<<r<<endl;
        arr[i][0] = Xor_arr(arr[i][0], Mul(q, arr[(i + 1) % 2][0]));
        arr[i][1] = Xor_arr(arr[i][1], Mul(q, arr[(i + 1) % 2][1]));
        arr[i][2] = r;
        //cout<<i<<" "<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<endl;
        //system("pause");
        if (arr[i][2] == "00000001")
            return arr[i][1];
        i = (i + 1) % 2;

    }
    return "";//正常逆元小于256
}


int main(void) {

//	for(int i=0;i<256;i++){
//		string r=Dec_to_Hex(i);
//		r=Mul_inverse(r);
//		if(r!="") cout<<i<<"\t 逆元："<<Hex_to_Dec(B_to_Hex(r))<<"\t "<<Hex_to_Dec(S_Box(r))<<endl;
//	}

    string r = Mul_inverse("1e");
    if (r != "") cout << "Inverse: " << B_to_Hex(r) << endl;
}


//两个多项式 异或
string Xor_arr(string a, string b) {
    if (a.length() != 8 || b.length() != 8)  //数出错
    {
        cout << "两个多项式 异或出错" << endl;
        exit(1);
    }
    for (int i = 0; i < 8; i++)//逐位异或
        a[i] = Xor(a[i], b[i]);
    return a;
}

//2进制表示转16进制
string B_to_Hex(string a) {
    if (a.length() != 8)  //数出错
        exit(1);
    string r = "";
    for (int i = 0; i <= 4; i = i + 4) {
        string t = a.substr(i, 4);
        if (t == "0000") r += '0';
        if (t == "0001") r += '1';
        if (t == "0010") r += '2';
        if (t == "0011") r += '3';
        if (t == "0100") r += '4';
        if (t == "0101") r += '5';
        if (t == "0110") r += '6';
        if (t == "0111") r += '7';
        if (t == "1000") r += '8';
        if (t == "1001") r += '9';
        if (t == "1010") r += 'A';
        if (t == "1011") r += 'B';
        if (t == "1100") r += 'C';
        if (t == "1101") r += 'D';
        if (t == "1110") r += 'E';
        if (t == "1111") r += 'F';
    }
    if (r.length() != 2)  //数出错
        exit(1);
    return r;
}

//将16进制的字符串转10进制
int Hex_to_Dec(string s) {
    int a;
    stringstream ss;
    ss << hex << s;    //以16进制读入流中
    ss >> a;        //10进制int型输出
    return a;
}

//将16进制的字符串转2进制的字符串 
string Hex_to_B(string num) {//num为十六进制
    string a = "";
    if (num.length() != 2)  //输入数出错
        exit(1);
    for (int i = 0; i < 2; i++) {
        switch (num[i]) {//num[0]为16的一次方位
            case '0':
                a += "0000";
                break;
            case '1':
                a += "0001";
                break;
            case '2':
                a += "0010";
                break;
            case '3':
                a += "0011";
                break;
            case '4':
                a += "0100";
                break;
            case '5':
                a += "0101";
                break;
            case '6':
                a += "0110";
                break;
            case '7':
                a += "0111";
                break;
            case '8':
                a += "1000";
                break;
            case '9':
                a += "1001";
                break;
            case 'A':
            case 'a':
                a += "1010";
                break;
            case 'B':
            case 'b':
                a += "1011";
                break;
            case 'C':
            case 'c':
                a += "1100";
                break;
            case 'D':
            case 'd':
                a += "1101";
                break;
            case 'E':
            case 'e':
                a += "1110";
                break;
            case 'F':
            case 'f':
                a += "1111";
                break;
            default:
                cout << "Hex_to_B出错" << endl;
                exit(1);
        }
    }
    return a;
}

//将10进制转16进制的字符串
string Dec_to_Hex(int a) {
    string s;
    stringstream ss;
    ss << hex << a;         //10进制转成十六进制读入流中，，再以字符串输出
    ss >> s;
    if (s.length() == 1) s = '0' + s;
    return s;
}

////求乘法逆元 
//int Mul_inverse(string s){
//	if(s=="00") return 0;//00的 逆元 为0 
//	if(s=="01") return 1;//01的 逆元 为1 
//	string arr[2][3];
//	arr[0][0]=“; arr[0][1]=“; arr[0][2]=Hex_to_Dec("FF");
//	arr[1][0]=0; arr[1][1]=1; arr[1][2]=Hex_to_Dec(s);
//	int i=0;
//	int n=0; 
//	while(true){
//		n=arr[i][2] / arr[(i+1)%2][2]; 
//		arr[i][0]=arr[i][0] - n*arr[(i+1)%2][0];
//		arr[i][1]=arr[i][1] - n*arr[(i+1)%2][1];
//		arr[i][2]=arr[i][2] - (n*arr[(i+1)%2][2]);
//		cout<<i<<" "<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<endl;
//		system("pause");
//		if(arr[i][2] ==1 ||arr[i][2] ==0){
//			return arr[i][1]>0 ? arr[i][1]:arr[i][1]+283;
////			return arr[i][1];
//		}
//		i=(i+1) % 2;
//		
//	}
//	return 999;//正常逆元小于256 
//}

//左移n位 
string Left_n(string a, int n) {
    if (a.length() != 8)  //数出错
    {
        cout << a << "左移n位 出错" << endl;
        exit(1);
    }
    for (int i = 0; i < n; i++)
        a += '0';
    return a.substr(n, 8);
}

//1*8  乘  8*1 
char Mul_(string a, string b) {
    int n = 0;
    for (int j = 0; j < 8; j++)
        if (a[j] == '1' && b[j] == '1') n++;
    return n % 2 == 0 ? '0' : '1';
}

////仿射变换
//string S_Box(string a){
//	string res="00000000";
//	for(int i=0;i<8;i++){//乘矩阵x
//		res[i]=Mul(x[i],a);
//	}
////	cout<<res<<endl;
//	//最后异或 63  "01100011" 
//	for(int i=0;i<8;i++){
//		res[i] = (res[i]==end[i]?'0':'1');
//	}
//	return B_to_Hex(res);	
//} 

//位异或运算 
char Xor(char n1, char n2) {
    return n1 == n2 ? '0' : '1';
}








