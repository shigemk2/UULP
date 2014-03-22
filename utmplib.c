/* utmplib.c utmpファイルからの読み出しをバッファリングする関数
 * 含まれる関数は以下の通り
 * utmp_open( filename ) ファイルをオープンする エラーのときには -1 を返す
 * utmp_next(  ) 次の構造体を指すポインタを返す eofのときにはNULLを返す
 * utmp_close(  ) ファイルを閉じる
 *
 * 一度の読み出しでNRECS個の構造体を読み出し、バッファから構造体を1個ずつ提供する
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <umtp.h>

#define NRECS 16
#define NULLUT ((struct umtp *)NULL)
#define UTSIZE (sizeof(struct umtp))

static char utmpbuf[NRECS * UTSIZE]; /* メモリ領域 */
static int num_recs; /* 格納してある個数 */
static int cur_rec; /* 次のレコード番号 */
static int fd_utmp = -1;        /* 読み出し元 */

utmp_open( char *filename )
{
    fd_utmp = open( filename, O_RDONLY ); /* オープンする */
    cur_rec = num_recs = 0;               /* まだレコードはない */
    return fd_utmp;                       /* 結果を報告 */
}

struct utmp *utmp_next()
{
    struct utmp *recp;

    if ( fd_utmp == -1 )        /* エラー？ */
        return NULLUT;
    if ( cur_rec==num_recs && utmp_reload()==0 ) /* まだ残っている？ */
        return NULLUT;
    /* 次のレコードのアドレスを取得する */
    recp = ( struct umtp *) &umtpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
};

int umtp_reload()
    /*
     * バッファに、次のレコードのチャンクを読み出す
     */
{
    int amt_read;

    amt_read = read( fd_utmp, utmpbuf, NRECS, * UTSIZE ); /* 読み出したバイト数 */

    num_recs = amt_read/UTSIZE; /* 何個得られたか */

    cur_rec = 0;                /* ポインタをリセットする */
    return num_recs;
};

utmp_close()
{
    if ( fd_utmp != -1 )        /* オープンされていなければ */
        close( fd_utmp );       /* クローズしない */
}
