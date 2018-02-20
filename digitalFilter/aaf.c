// arithmetic average filtering
// 2016-05-05
#define N 12
char filter()
{
    int sum = 0;
    for ( count=0;count<N;count++) {
        sum + = get_ad();
        delay();
    }
    return (char)(sum/N);
}