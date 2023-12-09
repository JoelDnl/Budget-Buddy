# Some settings for the Makefile.
#
# In particular, you might need to adjust WT_BASE to wherever Wt was installed. /usr/local is the default.

CXX=g++
WT_BASE=/usr/local
CXXFLAGS=--std=c++14 -I$(WT_BASE)/include
LDFLAGS=-L$(WT_BASE)/lib -Wl,-rpath,$(WT_BASE)/lib -lwthttp -lwt

# Identify our code base to be building. Headers are listed under DEPS, and objects under OBJS.

DEPS = add_income.h currency_conversion.h FinancialManagerApp.h UserManager.h User.h expense_reminder.h
OBJS = main.o FinancialManagerApp.o add_income.o currency_conversion.o UserManager.o User.o expense_reminder.o

# How do we build objects?

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# How do we build the executable?

app: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# How do we clean up after ourselves?

clean:
	rm -f app $(OBJS)
