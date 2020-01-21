#include <iostream>
#include <sstream>
#include <locale>
#include <vector>
#include <iterator>
#include <math.h>
using namespace std;

//Code for parsing ip string
struct dot_ws : ctype<char> 
{
    static const mask* make_table() 
    {
        static vector<mask> v(classic_table(), classic_table() + table_size);
        v['.'] |= space;
        v['/'] |= space;
        return &v[0];
    }
    dot_ws(size_t refs = 0) : ctype(make_table(), false, refs) {}
};

int subnet_calculator(int a)
{
    int num;

    if(a == 1)
        num = 128;
    if(a == 2)
        num = 192;
    if(a == 3)
        num = 224;
    if(a == 4)
        num = 240;
    if(a == 5)
        num = 248;
    if(a == 6)
        num = 252;
    if(a == 7)
        num = 254;

    return num;
}

int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

int main()
{
    //Declaring and storing the IP entered by the user.
    string ip;
    char ipclass;
    int net_address_loop = 0, temp, subnet_mask, subnet_loop;

    cout << "Enter an IP address: ";
    cin >> ip;

    ///////////PARSING IP ADDRESS///////////////////
    istringstream ins(ip);
    ins.imbue(locale(ins.getloc(), new dot_ws()));
    istream_iterator<long> beg(ins), end;
    vector<long> vec(beg, end);

    /////////FINDING THE CLASS OF THE IP////////////
    if(vec[0] <= 127)
    {
        ipclass = 'A';
        net_address_loop = 1;
    }
    else if(vec[0] >= 128 && vec[0] <= 191)
    {
        ipclass = 'B';
        net_address_loop = 2;
    }
    else if(vec[0] >= 192 && vec[0] <= 223)
    {
        ipclass = 'C';
        net_address_loop = 3;
    }

    ////////////FINDING THE SUBNET MASK///////////
    subnet_mask = vec[4] / 8;
    subnet_loop = subnet_mask;
    subnet_mask = vec[4] - (8 * subnet_mask);
    subnet_mask = subnet_calculator(subnet_mask);

    ///////////////CALCULATING SUBNET/////////////
    int sub_mask[8];
    int ip_add[8];
    int ip_add2[8];
    int sub_ip_add[8];
    int mask = subnet_mask;
    int subnet, counter = 0;
    stringstream ss;

    for(int i = 0; mask > 0; i++)    
    {    
        sub_mask[i] = mask % 2;    
        mask /= 2;  
    }

    if(vec[4] < 8)
        subnet = vec[0];
    if(vec[4] >= 8 && vec[4] < 16)
        subnet = vec[1];
    if(vec[4] >= 16 && vec[4] < 24)
        subnet = vec[2];
    if(vec[4] >= 24 && vec[4] < 32)
        subnet = vec[3];
    
    for(int i = 0; subnet > 0; i++)    
    {    
        sub_ip_add[i] = subnet % 2;    
        subnet /= 2;  
    }

    for(int i = 0; i < 8; i++)
    {
        if(sub_ip_add[i] == 1 || sub_ip_add[i] == 0)
        {

        }
        else
        {
            sub_ip_add[i] = 0;
        }
    }

    for(int i = 0; i < 8; i++)
    {
        if(sub_ip_add[i] == 1 && sub_mask[i] == 1)
        {
            ip_add[i] = 1;
        }
        else
        {
            ip_add[i] = 0;
        }
    }

    for(int i = 7; i >= 0; i--)
    {
        ip_add2[counter] = ip_add[i];
        counter++;
    }

    for (unsigned i = 0; i < sizeof ip_add2 / sizeof ip_add2[0]; ++i)
        ss << ip_add2[i];
    
    ss >> subnet;

    subnet = convertBinaryToDecimal(subnet);

    ////////CALCULATING SUBNET BROADCAST//////////
    int broadcast, b_temp;

    broadcast = 256 - subnet_mask;

    broadcast = broadcast + subnet - 1;
    ////////////////PRINTING INFO/////////////////
    
    cout << "Class Type: " << ipclass << endl;

    cout << "Classfull Network Address: ";
    if(net_address_loop == 1)
        cout << vec[0] << ".0.0.0" << endl;
    if(net_address_loop == 2)
        cout << vec[0] << "." << vec[1] << ".0.0" << endl;
    if(net_address_loop == 3)
        cout << vec[0] << "." << vec[1] << "." << vec[2] << ".0" << endl;

    cout << "Classfull Broadcast Address: ";
    if(net_address_loop == 1)
        cout << vec[0] << ".255.255.255" << endl;
    if(net_address_loop == 2)
        cout << vec[0] << "." << vec[1] << ".255.255" << endl;
    if(net_address_loop == 3)
        cout << vec[0] << "." << vec[1] << "." << vec[2] << ".255" << endl;

    cout << "Subnet Mask: ";
    if(subnet_loop == 0)
        cout << subnet_mask << ".0.0.0" << endl;
    else if(subnet_loop == 1)
        cout << "255." << subnet_mask << ".0.0" << endl;
    else if(subnet_loop == 2)
        cout << "255.255." << subnet_mask << ".0" << endl;
    else if(subnet_loop == 3)
        cout << "255.255.255." << subnet_mask << endl;
    
    cout << "Subnet: ";
    if(vec[4] < 8)
        cout << subnet << ".0.0.0" << endl;
    if(vec[4] >= 8 && vec[4] < 16)
        cout << vec[0] << "." << subnet << ".0.0" << endl;
    if(vec[4] >= 16 && vec[4] < 24)
        cout << vec[0] << "." << vec[1] << "." << subnet << ".0" << endl;
    if(vec[4] >= 24 && vec[4] < 32)
        cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << subnet << endl;

    cout << "Subnet Broadcast: ";
    if(vec[4] < 8)
        cout << broadcast << ".255.255.255" << endl;
    if(vec[4] >= 8 && vec[4] < 16)
        cout << vec[0] << "." << broadcast << ".255.255" << endl;
    if(vec[4] >= 16 && vec[4] < 24)
        cout << vec[0] << "." << vec[1] << "." << broadcast << ".255" << endl;
    if(vec[4] >= 24 && vec[4] < 32)
        cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << broadcast << endl;
        
    cout << "Number of usable Hosts: ";
    int hosts;
    hosts = 32 - vec[4];
    hosts = pow(2, hosts);
    cout << hosts - 2 << endl;

    cout << "Number of usable Subnets: ";
    int subnets = 32-vec[4];
    if(ipclass == 'A')
        subnets = 24 - subnets;
    if(ipclass == 'B')
        subnets = 16 - subnets;
    if(ipclass == 'C')
        subnets = 8 - subnets;
    cout << pow(2, subnets) << endl;

    if(ipclass == 'A')
    {
        cout << "Host Range Starting: ";
        if(vec[4] < 8)
            cout << subnet << ".0.0.1" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << subnet << ".0.1" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << subnet+1 << ".1" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << subnet << endl;
    
        cout << "Host Range Ending: ";
        if(vec[4] < 8)
        cout << broadcast << ".255.255.254" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << broadcast << ".255.254" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << broadcast << ".254" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << broadcast-1 << endl;
    }

    if(ipclass == 'B')
    {
        cout << "Host Range Starting: ";
        if(vec[4] < 8)
            cout << subnet+1 << ".0.0.0" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << subnet+1 << ".0.0" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << subnet+1 << ".0" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << subnet+1 << endl;
    
        cout << "Host Range Ending: ";
        if(vec[4] < 8)
        cout << broadcast-1 << ".255.255.255" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << broadcast-1 << ".255.255" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << broadcast-1 << ".255" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << broadcast-1 << endl;
    }

    if(ipclass == 'C')
    {
        cout << "Host Range Starting: ";
        if(vec[4] < 8)
            cout << subnet+1 << ".0.0.0" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << subnet+1 << ".0.0" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << subnet+1 << ".0" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << subnet+1 << endl;
    
        cout << "Host Range Ending: ";
        if(vec[4] < 8)
        cout << broadcast-1 << ".255.255.255" << endl;
        if(vec[4] >= 8 && vec[4] < 16)
            cout << vec[0] << "." << broadcast-1 << ".255.255" << endl;
        if(vec[4] >= 16 && vec[4] < 24)
            cout << vec[0] << "." << vec[1] << "." << broadcast-1 << ".255" << endl;
        if(vec[4] >= 24 && vec[4] < 32)
            cout << vec[0] << "." << vec[1] << "." << vec[2] << "." << broadcast-1 << endl;
    }

    cout << "Is This A Usable Host: ";
    int failure = 0;
    if(vec[4] < 8)
        if(vec[0] < subnet+1)
            failure++;
    if(vec[4] >= 8 && vec[4] < 16)
        if(vec[1] < subnet+1)
            failure++;
    if(vec[4] >= 16 && vec[4] < 24)
        if(vec[2] < subnet+1)
            failure++;
    if(vec[4] >= 24 && vec[4] < 32)
        if(vec[3] < subnet+1)
            failure++;
    if(ipclass == 'A')
    {
        if(vec[4] < 8)
            if(vec[0] > broadcast-1)
                failure++;
        if(vec[4] >= 8 && vec[4] < 16)
            if(vec[1] > broadcast-1)
                failure++;
        if(vec[4] >= 16 && vec[4] < 24)
            if(vec[2] > broadcast)
                failure++;
        if(vec[4] >= 24 && vec[4] < 32)
            if(vec[3] > broadcast-1)
                failure++;
    }
    else
    {
        if(vec[4] < 8)
            if(vec[0] > broadcast-1)
                failure++;
        if(vec[4] >= 8 && vec[4] < 16)
            if(vec[1] > broadcast-1)
                failure++;
        if(vec[4] >= 16 && vec[4] < 24)
            if(vec[2] > broadcast-1)
                failure++;
        if(vec[4] >= 24 && vec[4] < 32)
            if(vec[3] > broadcast-1)
                failure++;
    }

    if(failure == 0)
        cout << "Yes!" << endl;
    else
        cout << "No!" << endl;
    
    return 0;
}