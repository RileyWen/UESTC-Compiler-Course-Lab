
int a = 33, b, c = 44;
int bb = 3;
str aaa = "12345", bbb;

str f()
{
	str b = "aaa" + "ddd";
	str c = "ccc" + "bb";
	//print c;
	return c;
}


int func1(int fir, int sec)
{
	int b = fir*sec + 3000;
	int a = 10;
	while(a > 0)
	{
		str k = "\t" + a + "\n" ;
		a = a -1;
	}
	//print "func1 done" + "\n" ;
	return b;
}


void multable()
{
	int i = 1;
	while(i < 10)
	{
		int j = 1;
		while(j < 10)
		{
			print "\t" + i*j;
			j = j + 1;
		}
		print "\n";
		i = i +1;
	}
}

int fn(int n)
{
	if(n == 1)
		return 1;
	else
	{
		int r = n* fn(n-1);
		print r;
		return r;
	}
}


int main()
{
	multable();
	return 0;
}
