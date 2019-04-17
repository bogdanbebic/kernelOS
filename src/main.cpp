#include <DOS.H>

/*
 *  User application to be run in the kernel
 */
int userMain(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    // TODO: IVT initialization

    // TODO: initialize kernel

    // TODO: run thread on userMain
    int ret_userMain = userMain(argc, argv);

    // TODO: cleanup kernel

    // IVT restore

    return ret_userMain;
}
