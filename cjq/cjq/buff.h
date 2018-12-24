#pragma once
#include <iostream>
using namespace std;

struct buff {	
	double max_time;//buff ������ʱ��
	double time;    //buff ʣ�����ʱ��
	double max_cd;  //buff �����ȴʱ��	
	double cooldown;//buff ʣ����ȴʱ��
	double add_c;   //buff �����ӳ�
	double add_d;   //buff �˺��ӳ�
	double add_d_v; //buff �ض������˺�(�þ�������֮��)
};

class buff_all {
	buff Avenging_Wrath{ 30,0,120,0,0.2,0.2,0 }; //����֮ŭ
	buff Just_Verdict{ 6,0,0,0,0,0,0.15 };       //����þ�
	buff Holy_DivinePurpose{ 10,0,0,0,0,0,0 };   //��ʥ��־
	buff Judgement{ 15,0,0,0,0,0,0.25 };         //����
	buff Blade_of_Fury{ 10,0,0,0,0,0,0.25 };
	buff* get_name(string str) {
		if (str == "Avenging_Wrath") return &Avenging_Wrath;		
		if (str == "Just_Verdict") return &Just_Verdict;
		if (str == "Holy_DivinePurpose") return &Holy_DivinePurpose;
		if (str == "Judgement") return &Judgement;
		if (str == "Blade_of_Fury") return &Blade_of_Fury;
	}

	void cutdown(buff* b, double gcd)
	{
		if (b->cooldown > gcd) b->cooldown = b->cooldown - gcd; else b->cooldown = 0;
		if (b->time > gcd) b->time = b->time - gcd; else b->time = 0;
	}

	void clear_buff(buff *b) { b->time = 0; }

public:
	buff_all() {};
	double get_time(string str) { buff* b = get_name(str); return b->time; };
	double get_cooldown(string str) { buff* b = get_name(str); return b->cooldown; };
	double get_add_c(string str) { buff* b = get_name(str); return b->add_c; };
	double get_add_d(string str) { buff* b = get_name(str); return b->add_d; };
	double get_add_d_v(string str) { buff* b = get_name(str); return b->add_d_v; };

	void set_buff(string str) {
		buff* b = get_name(str);
		if (str == "Avenging_Wrath")
		{
			b->cooldown = 120;
			b->time = 30;
		}
		else if (str == "Just_Verdict")	b->time = 6;
		else if (str == "Holy_DivinePurpose") b->time = 10;		
		else if (str == "Judgement") b->time = 15;
		else if (str == "Blade_of_Fury") b->time = 10;
	};	

	void update(double gcd,bool b_v, bool b_j)
	{
		if (b_v)
		{
			clear_buff(&Holy_DivinePurpose);
			clear_buff(&Judgement);
		}
		if (b_j)
			clear_buff(&Blade_of_Fury);
		cutdown(&Avenging_Wrath, gcd);
		cutdown(&Just_Verdict, gcd);
		cutdown(&Holy_DivinePurpose, gcd);
		cutdown(&Judgement, gcd);
		cutdown(&Blade_of_Fury, gcd);
	};
};
