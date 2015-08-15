#include "afxwin.h"
#include<string.h>


#define vocung -1

typedef struct
{
    int a;
    int b;
}dt;




class CMyLib:public CWinApp
{
public:
	CMyLib()
	{}
	
};

CMyLib theLib;

BOOL lien_thong(int s, dt *u, int *mangtg, int *daxet, int **e, int sodinh)
{
	int i;
	for(i=0;i<sodinh;i++)
	{
		daxet[i]=0;
		u[i].a=0;
		u[i].b=0;
		mangtg[i]=-1;
	}
	s=0;
	int v,j;
    daxet[s]=1;
    mangtg[0]=s;
	
    while(mangtg[0]!=-1)
    {
        j=0;
        while(j<sodinh && mangtg[j]!=-1)
        {
            j++;
        }
        v=mangtg[j-1];
        mangtg[j-1]=-1;
        for(i=0;i<sodinh;i++)
        {
            if(e[v][i]>0 && daxet[i]==0)
            {
                j=0;
                while(mangtg[j]!=-1 && j<sodinh)
                {
                    j++;
                }
                while(j>0)
                {
                    mangtg[j]=mangtg[j-1];
                    j--;
                }
                mangtg[0]=i;
                daxet[i]=1;
            }
        }
		if(mangtg[0]==-1)
		{
			int dem=0;
			for(i=0;i<sodinh;i++)
			{
				if(daxet[i]==1)
				{
					dem++;
				}
			}
			if(dem<sodinh)
			{
				for(i=0;i<sodinh;i++)
				{
					daxet[i]=0;
					u[i].a=0;
					u[i].b=0;
				}
				for(i=0;i<sodinh;i++)
				{
					mangtg[i]=-1;
				}
				return FALSE;
			}
		}
    }
	for(i=0;i<sodinh;i++)
	{
		daxet[i]=0;
		u[i].a=0;
		u[i].b=0;
		mangtg[i]=-1;
	}
	return TRUE;
	
}

void tim_min(int sodinh, int *daxet, dt *u, int *vtmin, int i)
{
	while(i<sodinh)
	{
		if(daxet[i]==0 && u[i].b>0 && u[i].b<u[*vtmin].b)
		{
			*vtmin=i;
		}
		i++;
    }
}

void dao_mang(int i, int t, int *mangtg, char *str, dt *u, int s, int sodinh)
{
	i=t;
	int j=0;
	while(i!=s)
	{
		mangtg[j]=i;
		j++;
		i=u[i].a;
	}
	mangtg[j]=s;
	j=0;
	while(mangtg[j]!=-1 && j<sodinh)
	{
		j++;
	}
	char stg[10];
	str[0]='\0';
	j--;
	while(j>=0)
	{
		if(j>0)
			sprintf(stg,"%c -> ",mangtg[j]+65);
		else
			sprintf(stg,"%c",mangtg[j]+65);
		strcat(str,stg);
		j--;
	}
	strcat(str,".");
	//--------------
}


int min_prim(int i, int vtmin, int sodinh, int *daxet, dt *u)
{
	i=0;
	vtmin=0;
	while(i<=sodinh && (daxet[i]==1 || u[i].b==vocung || u[i].b==0))
	{
		i++;
		vtmin++;
	}
	while(i<sodinh)
	{
		if(daxet[i]==0)
		{
			if(u[i].b>0)
			{
				if(u[i].b<u[vtmin].b) 
				{
					vtmin=i;
				}
			}
		}
		i++;
	}
	return vtmin;
}