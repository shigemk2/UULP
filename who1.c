/* who1.c whoプログラムの最初のバージョン
 * utmpファイルをオープンし、読み出した結果を表示する
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST                /* 出力にリモートマシン名を組込む */

int main()
{
    struct utmp current_record; /* 情報をここに読み込む */
    int utmpfd;                 /* このデスクリプタから読み出す */
    int reclen = sizeof(current_record);

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror( UTMP_FILE );    /* UTMP_FILEはutmp.hで定義されている */
        exit(1);
    }

    while ( read(utmpfd, &current_record, reclen) == reclen )
        show_info(&current_record);
    close(utmpfd);
    return 0;                   /* うまくいった */
}

/*
 * show_info()
 * utmp構造体の内容を人間が読めるように表示する
 * *注意* サイズ情報をハードコードしてはならない
*/
show_info( struct  utmp *utbufp )
{
    printf ("%-8.8s", utbufp->ut_name); /* ログイン名 */
    printf (" "); /* スペース */
    printf ("%-8.8s", utbufp->ut_line); /* tty */
    printf (" "); /* スペース */
    printf ("%-101d", utbufp->ut_time); /* ログイン時刻 */
    printf (" "); /* スペース */
#ifdef SHOWHOST
    printf ("(%s)", utbufp->ut_host); /* ホスト */
#endif
    printf ("\n"); /* 改行 */
};
