#include <Wt/WApplication.h>
#include "FinancialManagerApp.h"

int main(int argc, char **argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
        return std::make_unique<FinancialManagerApp>(env);
    });
}
