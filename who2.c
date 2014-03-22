/* who1.c whoプログラムの最初のバージョン
 * utmpファイルをオープンし、読み出した結果を表示する
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST                /* 出力にリモートマシン名を組込む */

void showtime(long);
void show_info(struct utmp *);

int main()
{
    struct utmp utbuf; /* 情報をここに読み込む */
    int utmpfd;                 /* このデスクリプタから読み出す */

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror( UTMP_FILE );    /* UTMP_FILEはutmp.hで定義されている */
        exit(1);
    }

    while ( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
        show_info(&utbuf);
    close(utmpfd);
    return 0;                   /* うまくいった */
}

/*
 * show_info()
 * utmp構造体の内容を人間が読めるように表示する
 * レコードにユーザー名が含まれていない場合は何も表示しない
*/
void show_info( struct  utmp *utbufp )
{
    if ( utbufp->ut_type != USER_PROCESS )
        return;
    printf ("%-8.8s", utbufp->ut_name); /* ログイン名 */
    printf (" ");               /* スペース */
    printf ("%-8.8s", utbufp->ut_line); /* tty */
    printf (" ");               /* スペース */
    showtime( utbufp->ut_time ); /* 時刻の表示 */
    printf (" ");               /* スペース */
#ifdef SHOWHOST
    printf ("(%s)", utbufp->ut_host); /* ホスト */
#endif
    printf ("\n"); /* 改行 */
};

/*
 * showtime()
 * 人間が理解しやすい形式で時刻を表示する
 * ctimeを使って文字列を組み立ててからその一部を抜き出す
 * 注意: %12.12sは文字列をchar12個分で出力し、
 * 長さを12バイト以下に制限する
*/
void showtime( long timeval )
{
    char *cp;                        /* 時刻のアドレスを保持するポインタ */
    cp = ctime(&timeval);
    printf("%12.12s", cp+4); /* 位置4から12個分を抜き出す */
};
