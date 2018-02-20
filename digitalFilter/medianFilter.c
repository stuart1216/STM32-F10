% median filter
% 2016-05-05
#define N 11
char filter()
{
        char value_buf[N];
        char count,i,j,temp;
        for ( count=0;count<N;count++)
        {
                value_buf[count] = get_ad();
                delay();
        }
        for (j=0;j<N-1;j++)
        {
                for (i=0;i<N-j;i++)
                {
                        if ( value_buf>value_buf[i+1] )
                        {
                                temp = value_buf;
                                value_buf = value_buf[i+1]; 
                                value_buf[i+1] = temp;
                        }
                }
        }
return value_buf[(N-1)/2];
} 