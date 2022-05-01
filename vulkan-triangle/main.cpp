#include <stdio.h>
#include <string.h>
#include <string>

#include "test_common.h"

std::vector<TestMetadata> &test_list()
{
  static std::vector<TestMetadata> list;
  return list;
}

void check_tests(int argc, char **argv)
{
  std::vector<TestMetadata> &tests = test_list();

  for(TestMetadata &test : tests)
    test.test->Prepare(argc, argv);
}

void RegisterTest(TestMetadata test)
{
  test_list().push_back(test);
}

int main(int argc, char **argv)
{
  std::vector<TestMetadata> &tests = test_list();

  std::sort(tests.begin(), tests.end());

  if(argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h") || !strcmp(argv[1], "-?") ||
                   !strcmp(argv[1], "/help") || !strcmp(argv[1], "/h") || !strcmp(argv[1], "/?")))
  {
    printf(R"(Testing demo program

Usage: %s Test_Name [test_options]

  --help                        Print this help message.
  --list                        Lists all tests, with name, API, description, availability.
  --list-raw                    Lists the available test names only, one per line.
  --validate
  --debug                       Run the demo with API validation enabled.
  --width / -w                  Specify the window width.
  --height / -h                 Specify the window height.
  --frames <n>
  --max-frames <n>
  --frame-count <n>             Only run the demo for this number of frames
)",
           argc == 0 ? "demos" : argv[0]);

    fflush(stdout);
    return 1;
  }

  if(argc >= 2 && !strcmp(argv[1], "--list"))
  {
    check_tests(argc, argv);

    TestAPI prev = TestAPI::Count;

    for(const TestMetadata &test : tests)
    {
      if(test.API != prev)
      {
        if(prev != TestAPI::Count)
          printf("\n\n");
        printf("======== %s tests ========\n\n", APIName(test.API));
      }

      prev = test.API;

      printf("%s: %s", test.Name, test.IsAvailable() ? "Available" : "Unavailable");

      if(!test.IsAvailable())
        printf(" because %s", test.AvailMessage());

      printf("\n\t%s\n\n", test.Description);
    }

    fflush(stdout);
    return 1;
  }

  if(argc >= 2 && !strcmp(argv[1], "--list-raw"))
  {
    check_tests(argc, argv);

    // output TSV
    printf("Name\tAvailable\tAvailMessage\n");

    for(const TestMetadata &test : tests)
    {
      printf("%s\t%s\t%s\n", test.Name, test.IsAvailable() ? "True" : "False",
             test.IsAvailable() ? "Available" : test.AvailMessage());
    }

    fflush(stdout);
    return 1;
  }

  if(tests.empty())
  {
    fprintf(stderr, "No tests registered\n");
    fflush(stderr);
    return 1;
  }

  // Check if the first arg is a valid test name. If it isn't,
  // allow the UI to appear, so that flags can be used with the UI
  bool validTestArg = false;
  if(argc >= 2)
  {
    for(const TestMetadata &test : tests)
    {
      if(!strcmp(test.Name, argv[1]))
      {
        validTestArg = true;
        break;
      }
    }
  }

  std::string testchoice;
  if(tests.size() == 1)
  {
    // if there's only one test we've probably hardcoded this for a repro. Launch it
    testchoice = tests[0].Name;
  }
  else if(validTestArg)
  {
    testchoice = argv[1];
  }

  if(testchoice.empty())
    return 0;

  for(const TestMetadata &test : tests)
  {
    if(testchoice == test.Name)
    {
      TEST_LOG("Running '%s'", test.Name);
      test.test->Prepare(argc, argv);
      test.test->SetName(test.Name);

      if(!test.IsAvailable())
      {
        TEST_ERROR("%s is not available: %s", test.Name, test.test->Avail.c_str());
        return 5;
      }

      int ret = test.test->main();
      test.test->Shutdown();
      return ret;
    }
  }

  TEST_ERROR("%s is not a known test", argv[1]);

  return 2;
}
