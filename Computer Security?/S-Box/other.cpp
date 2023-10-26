#include<iostream>
#include<string>
#include<sstream>

using namespace std;

string m = "11B";
string f = "";
string end = "01100011";//63����
//string x[8];//����x
////��ʼ�� X
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

//��16���Ƶ��ַ���ת2���Ƶ��ַ��� 
string Hex_to_B(string num);

//2���Ʊ�ʾת16����
string B_to_Hex(string a);

//��16���Ƶ��ַ���ת10����
int Hex_to_Dec(string s);

//��10����ת16���Ƶ��ַ���
string Dec_to_Hex(int a);

//��10����ת16���Ƶ��ַ���
string Dec_to_Hex(int a);

//1*8  ��  8*1
char Mul_(string a, string b);

//λ�������
char Xor(char n1, char n2);

//����nλ
string Left_n(string a, int n);

//��������ʽ ���
string Xor_arr(string a, string b);

//����任
string S_Box(string a);

//gf(2,8)�˷�
string Mul(string a, string b) {
    string temp[8];//�洢�м���
    //�����м���
    temp[0] = a;
    for (int i = 1; i < 8; i++) {
        temp[i] = Left_n(temp[i - 1], 1);
        if (temp[i - 1][0] == '1')//�ж��Ƿ���Ҫ���1B
            temp[i] = Xor_arr(temp[i], "00011011");
    }
    //������
    string res = "00000000";
    //����g ����f���м������
    for (int i = 0; i < 8; i++) {
        if (b[i] == '1')
            res = Xor_arr(res, temp[7 - i]);
    }
    return res;
}

//gf(2,8)����
void Div(string a, string b, string &r, string &s) {
    r = b;
    s = "00000000"; //r����  s��
    bool flag = false;
    if (a == "100011011") { flag = true; }//һ��ʼ��11B
    string t = "00000000";
    while (true) {//��ʽ����ߴ��� С�� ��ʽ����ߴ��� ����
        int n = flag ? b.find('1') + 1 : a.find('1') - b.find('1');//����������ߴ� - ��������ߴ�
        n = n > 0 ? n : -n;
        if (flag) {
            flag = false;
            a = a.substr(1, 8);
        }
        r = Left_n(b, n);//���ƣ�ͬ�ߴ�
        //cout<<n<<"\t ���ƣ�"<<r<<"\t �̣�";
        s[7 - n] = '1';
        //cout<<s<<"\t ������" ;
        r = Xor_arr(r, a);
        //cout<<r<<endl;
        a = r;
        //system("pause");
        if (b.find('1') < r.find('1')) {
            //cout<<"���ϳ���������"<<endl;
            break;
        }
    }
}


//��˷���Ԫ 
string Mul_inverse(string s) {
    if (s == "00") return "00000000";//00�� ��Ԫ Ϊ0
    if (s == "01") return "00000001";//01�� ��Ԫ Ϊ1
    string arr[2][3];
    arr[0][0] = "00000001";
    arr[0][1] = "00000000";
    arr[0][2] = "100011011";
    arr[1][0] = "00000000";
    arr[1][1] = "00000001";
    arr[1][2] = Hex_to_B(s);
    //cout<<"��������"<<arr[0][2]<<"  ������"<<arr[1][2]<<endl;
    int i = 0;
    string q = "", r = "";//n�̣� r����
    while (true) {
        Div(arr[i][2], arr[(i + 1) % 2][2], r, q);
        //cout<<"�̣�"<<q<<"  ������"<<r<<endl;
        arr[i][0] = Xor_arr(arr[i][0], Mul(q, arr[(i + 1) % 2][0]));
        arr[i][1] = Xor_arr(arr[i][1], Mul(q, arr[(i + 1) % 2][1]));
        arr[i][2] = r;
        //cout<<i<<" "<<arr[i][0]<<" "<<arr[i][1]<<" "<<arr[i][2]<<endl;
        //system("pause");
        if (arr[i][2] == "00000001")
            return arr[i][1];
        i = (i + 1) % 2;

    }
    return "";//������ԪС��256
}


int main(void) {

//	for(int i=0;i<256;i++){
//		string r=Dec_to_Hex(i);
//		r=Mul_inverse(r);
//		if(r!="") cout<<i<<"\t ��Ԫ��"<<Hex_to_Dec(B_to_Hex(r))<<"\t "<<Hex_to_Dec(S_Box(r))<<endl;
//	}

    string r = Mul_inverse("1e");
    if (r != "") cout << "Inverse: " << B_to_Hex(r) << endl;
}


//��������ʽ ���
string Xor_arr(string a, string b) {
    if (a.length() != 8 || b.length() != 8)  //������
    {
        cout << "��������ʽ ������" << endl;
        exit(1);
    }
    for (int i = 0; i < 8; i++)//��λ���
        a[i] = Xor(a[i], b[i]);
    return a;
}

//2���Ʊ�ʾת16����
string B_to_Hex(string a) {
    if (a.length() != 8)  //������
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
    if (r.length() != 2)  //������
        exit(1);
    return r;
}

//��16���Ƶ��ַ���ת10����
int Hex_to_Dec(string s) {
    int a;
    stringstream ss;
    ss << hex << s;    //��16���ƶ�������
    ss >> a;        //10����int�����
    return a;
}

//��16���Ƶ��ַ���ת2���Ƶ��ַ��� 
string Hex_to_B(string num) {//numΪʮ������
    string a = "";
    if (num.length() != 2)  //����������
        exit(1);
    for (int i = 0; i < 2; i++) {
        switch (num[i]) {//num[0]Ϊ16��һ�η�λ
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
                cout << "Hex_to_B����" << endl;
                exit(1);
        }
    }
    return a;
}

//��10����ת16���Ƶ��ַ���
string Dec_to_Hex(int a) {
    string s;
    stringstream ss;
    ss << hex << a;         //10����ת��ʮ�����ƶ������У��������ַ������
    ss >> s;
    if (s.length() == 1) s = '0' + s;
    return s;
}

////��˷���Ԫ 
//int Mul_inverse(string s){
//	if(s=="00") return 0;//00�� ��Ԫ Ϊ0 
//	if(s=="01") return 1;//01�� ��Ԫ Ϊ1 
//	string arr[2][3];
//	arr[0][0]=��; arr[0][1]=��; arr[0][2]=Hex_to_Dec("FF");
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
//	return 999;//������ԪС��256 
//}

//����nλ 
string Left_n(string a, int n) {
    if (a.length() != 8)  //������
    {
        cout << a << "����nλ ����" << endl;
        exit(1);
    }
    for (int i = 0; i < n; i++)
        a += '0';
    return a.substr(n, 8);
}

//1*8  ��  8*1 
char Mul_(string a, string b) {
    int n = 0;
    for (int j = 0; j < 8; j++)
        if (a[j] == '1' && b[j] == '1') n++;
    return n % 2 == 0 ? '0' : '1';
}

////����任
//string S_Box(string a){
//	string res="00000000";
//	for(int i=0;i<8;i++){//�˾���x
//		res[i]=Mul(x[i],a);
//	}
////	cout<<res<<endl;
//	//������ 63  "01100011" 
//	for(int i=0;i<8;i++){
//		res[i] = (res[i]==end[i]?'0':'1');
//	}
//	return B_to_Hex(res);	
//} 

//λ������� 
char Xor(char n1, char n2) {
    return n1 == n2 ? '0' : '1';
}








