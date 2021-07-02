#include <gtest/gtest.h>

#include <ArgumentParser.h>

TEST(TestArgumentParser, RegisterFlag)
{
    ArgumentParser argParser;

    EXPECT_FALSE(argParser.isFlagRegistered("anyflag"));

    argParser.registerFlag("anyflag");
    EXPECT_TRUE(argParser.isFlagRegistered("anyflag"));
}

TEST(TestArgumentParser, RegisterFlag_Whitespace)
{
    ArgumentParser argParser;

    argParser.registerFlag("any flag");
    EXPECT_FALSE(argParser.isFlagRegistered("any flag"));
}

TEST(TestArgumentParser, RegisterOption)
{
    ArgumentParser argParser;

    EXPECT_FALSE(argParser.isOptionRegistered("anyoption"));

    argParser.registerOption("anyoption");
    EXPECT_TRUE(argParser.isOptionRegistered("anyoption"));
}

TEST(TestArgumentParser, RegisterOption_Whitespace)
{
    ArgumentParser argParser;

    argParser.registerOption("any option");
    EXPECT_FALSE(argParser.isOptionRegistered("any option"));
}

TEST(TestArgumentParser, Parse)
{
    ArgumentParser argParser;

    char argv0[] = "TestArgumentParser";
    char argv1[] = "--flag";
    char argv2[] = "--somestring=shouldwork";
    char argv3[] = "--number=7";
    char argv4[] = "--float=3.14";
    char* argv[] = { argv0, argv1, argv2, argv3, argv4 };

    argParser.registerFlag("flag");
    argParser.registerOption("somestring");
    argParser.registerOption("number");
    argParser.registerOption("float");

    argParser.parse(5, argv);

    EXPECT_FALSE(argParser.getFlag("TestArgumentParser"));
    EXPECT_TRUE(argParser.getFlag("flag"));
    EXPECT_EQ(argParser.getOptionAs<const std::string>("somestring"), "shouldwork");
    EXPECT_EQ(argParser.getOptionAs<int>("number"), 7);
    EXPECT_EQ(argParser.getOptionAs<float>("float"), 3.14f);
}
