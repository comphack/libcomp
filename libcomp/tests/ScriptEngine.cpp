/**
 * @file libcomp/tests/ScriptEngine.cpp
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Test the Squirrel scripting language interface.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2016-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Ignore warnings
#include <PushIgnore.h>

// Google Test Includes
#include <gtest/gtest.h>

// Sqrat Includes
#include <sqrat.h>

// Stop ignoring warnings
#include <PopIgnore.h>

// libcomp Includes
#include <Crypto.h>
#include <Log.h>
#include <Packet.h>
#include <ScriptEngine.h>
#include <TestObject.h>
#include <TestObjectA.h>
#include <TestObjectB.h>
#include <TestObjectC.h>
#include <TestObjectD.h>
#include <TestObjectE.h>

using namespace libcomp;

TEST(ScriptEngine, EvalCompileError) {
  ScriptEngine engine;

  int errorCount = 0;

  // Check for a compile error that produces a log message.
  Log::GetSingletonPtr()->AddLogHook(
      [](LogComponent_t comp, Log::Level_t level, const String& msg,
         void* pUserData) {
        (void)comp;
        (void)msg;

        EXPECT_EQ(level, Log::LOG_LEVEL_ERROR);

        (*reinterpret_cast<int*>(pUserData))++;
      },
      &errorCount);

  EXPECT_FALSE(engine.Eval("1=2"));
  EXPECT_EQ(errorCount, 1);

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, EvalRuntimeError) {
  ScriptEngine engine;

  int errorCount = 0;

  // Check for a runtime error that produces log messages.
  Log::GetSingletonPtr()->AddLogHook(
      [](LogComponent_t comp, Log::Level_t level, const String& msg,
         void* pUserData) {
        (void)comp;
        (void)msg;

        EXPECT_EQ(level, Log::LOG_LEVEL_ERROR);

        (*reinterpret_cast<int*>(pUserData))++;
      },
      &errorCount);

  EXPECT_FALSE(engine.Eval("FunctionThatDoesNotExist()"));
  EXPECT_NE(errorCount, 0);

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, EvalPrint) {
  ScriptEngine engine;

  int messageCount = 0;

  // Check for a call to print() that produces a log message.
  Log::GetSingletonPtr()->AddLogHook(
      [](LogComponent_t comp, Log::Level_t level, const String& msg,
         void* pUserData) {
        (void)comp;

        EXPECT_EQ(msg, "SQUIRREL: Test\n");
        EXPECT_EQ(level, Log::LOG_LEVEL_INFO);

        (*reinterpret_cast<int*>(pUserData))++;
      },
      &messageCount);

  EXPECT_TRUE(engine.Eval("print(\"Test\");"));
  EXPECT_EQ(messageCount, 1);

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ReadOnlyPacket) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<libcomp::Packet>();

  EXPECT_TRUE(
      engine.Eval("p <- Packet();\n"
                  "p.WriteBlank(3);\n"
                  "print(p.Size());\n"));
  EXPECT_EQ(scriptMessages, "SQUIRREL: 3\n");
  scriptMessages.Clear();

  EXPECT_TRUE(
      engine.Eval("p <- Packet();\n"
                  "print(p.Size());\n"));
  EXPECT_EQ(scriptMessages, "SQUIRREL: 0\n");
  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ReadWriteArray) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<libcomp::Packet>();

  EXPECT_TRUE(
      engine.Eval("p <- Packet();\n"
                  "local b = blob();\n"
                  "b.writen(-1095041334, 'i');"  // 0xCAFEBABE
                  "p.WriteArray(b);\n"
                  "if(4 == p.Size())\n"
                  "{\n"
                  "p.Rewind(4)\n"
                  "local c = p.ReadArray(4);\n"
                  "print(c.readn('i'));\n"
                  "}\n"));
  EXPECT_EQ(scriptMessages, "SQUIRREL: -1095041334\n");
  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, FunctionCall) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<libcomp::Packet>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "b <- Packet();\n"
                  "a.WriteU16Little(0x1234);\n"
                  "b.WriteU16Little(0x5678);\n"
                  "return b;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<libcomp::Packet>> ref;
  std::shared_ptr<libcomp::Packet> a(new libcomp::Packet);
  std::shared_ptr<libcomp::Packet> b;

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<libcomp::Packet>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  b = ref->GetSharedObject();

  ASSERT_TRUE(b);

  EXPECT_EQ(b->Size(), 2);
  EXPECT_EQ(a->Size(), 2);

  EXPECT_EQ(b->Tell(), 2);
  EXPECT_EQ(a->Tell(), 2);

  a->Rewind();
  b->Rewind();

  EXPECT_EQ(a->ReadU16Little(), 0x1234);
  EXPECT_EQ(b->ReadU16Little(), 0x5678);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, GeneratedObject) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObject>();

  EXPECT_TRUE(engine.Eval(
      "t <- TestObject();\n"
      "if(t.GetUnsigned8() == 100)\n"
      "{\n"
      "   error(\"Test value already set!\");\n"
      "}\n"
      "if(!t.SetUnsigned8(256))\n"  // Should be out of bounds and fail
      "{\n"
      "   t.SetUnsigned8(100);\n"
      "}\n"
      "print(t.GetUnsigned8());\n"
      "print(t.GetStringCP932());\n"
      "t.SetStringCP932(\"日本人\");\n"
      "print(t.GetStringCP932());\n"));
  EXPECT_EQ(scriptMessages,
            "SQUIRREL: 100\n"
            "SQUIRREL: 日本語\n"
            "SQUIRREL: 日本人\n");
  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ScriptA_ScriptB) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction()\n"
                  "{\n"
                  "local a = TestObjectA();\n"
                  "local b = TestObjectB();\n"
                  "a.SetValue(\"testA\");\n"
                  "a.SetObjectB(b);\n"
                  "b.SetValue(\"testB\");\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  std::shared_ptr<objects::TestObjectA> a;

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>();

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  a = ref->GetSharedObject();

  ASSERT_TRUE(a);

  EXPECT_EQ(a->GetValue(), "testA");
  ASSERT_TRUE(a->GetObjectB());
  EXPECT_EQ(a->GetObjectB()->GetValue(), "testB");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ServerA_ScriptB) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local b = TestObjectB();\n"
                  "a.SetValue(\"testA\");\n"
                  "a.SetObjectB(b);\n"
                  "b.SetValue(\"testB\");\n"
                  "return true;\n"
                  "}\n"));

  auto a = std::make_shared<objects::TestObjectA>();

  auto ret = Sqrat::RootTable(engine.GetVM())
                 .GetFunction("TestFunction")
                 .Evaluate<SQBool>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ret);

  EXPECT_EQ(a->GetValue(), "testA");
  ASSERT_TRUE(a->GetObjectB());
  EXPECT_EQ(a->GetObjectB()->GetValue(), "testB");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ServerA_ServerB) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a, b)\n"
                  "{\n"
                  "a.SetValue(\"testA\");\n"
                  "a.SetObjectB(b);\n"
                  "b.SetValue(\"testB\");\n"
                  "return true;\n"
                  "}\n"));

  auto a = std::make_shared<objects::TestObjectA>();
  auto b = std::make_shared<objects::TestObjectB>();

  auto ret = Sqrat::RootTable(engine.GetVM())
                 .GetFunction("TestFunction")
                 .Evaluate<SQBool>(a, b);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ret);

  EXPECT_EQ(a->GetValue(), "testA");
  ASSERT_TRUE(a->GetObjectB());
  EXPECT_EQ(a->GetObjectB()->GetValue(), "testB");
  EXPECT_EQ(a->GetObjectB(), b);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ScriptA_ServerB) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(b)\n"
                  "{\n"
                  "local a = TestObjectA();\n"
                  "a.SetValue(\"testA\");\n"
                  "a.SetObjectB(b);\n"
                  "b.SetValue(\"testB\");\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  std::shared_ptr<objects::TestObjectA> a;

  auto b = std::make_shared<objects::TestObjectB>();

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(b);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  a = ref->GetSharedObject();

  ASSERT_TRUE(a);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a->GetValue(), "testA");
  ASSERT_TRUE(a->GetObjectB());
  EXPECT_EQ(a->GetObjectB()->GetValue(), "testB");
  EXPECT_EQ(a->GetObjectB(), b);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, CString) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "a.SetValue(a.GetValue() + \"testObjA\");\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto a2 = ref->GetSharedObject();

  ASSERT_TRUE(a2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a, a2);
  EXPECT_EQ(a->GetValue(), "testOf_testObjA");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, DowncastChild) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();
  engine.Using<objects::TestObjectD>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a, c, d)\n"
                  "{\n"
                  "a.SetValue(c.GetValue() + \"_\" + c.GetExtraValue());\n"
                  "c.SetExtraValue(789);\n"
                  "a.SetObjectB(c);\n"
                  "a.SetObjectB(d);\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  auto c = std::make_shared<objects::TestObjectC>();
  c->SetValue("testObjB");
  c->SetExtraValue(123);

  auto d = std::make_shared<objects::TestObjectD>();
  d->SetValue(456);

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a, c, d);

  auto messageLines = scriptMessages.Split("\n");
  messageLines.erase(messageLines.begin());

  EXPECT_EQ(messageLines.front(),
            "ERROR: SQUIRREL: AN ERROR HAS OCCURED "
            "[wrong type (TestObjectB expected, got TestObjectD)]");

  ASSERT_FALSE(ref);

  EXPECT_EQ(a->GetValue(), "testObjB_123");
  EXPECT_EQ(std::dynamic_pointer_cast<objects::TestObjectC>(a->GetObjectB()),
            c);
  EXPECT_EQ(c->GetExtraValue(), 789);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, GetObjectList) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
    printf("%s", msg.C());
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local s = \"\";\n"
                  "\n"
                  "foreach(b in a.GetObjectBList())\n"
                  "{\n"
                  "s += b.GetValue();\n"
                  "}\n"
                  "\n"
                  "a.SetValue(s);\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto b1 = std::make_shared<objects::TestObjectB>();
  b1->SetValue("b1");
  auto c1 = std::make_shared<objects::TestObjectC>();
  c1->SetValue("c1");
  auto b2 = std::make_shared<objects::TestObjectB>();
  b2->SetValue("b2");
  std::list<std::shared_ptr<objects::TestObjectB>> objs;
  objs.push_back(b1);
  objs.push_back(c1);
  objs.push_back(b2);
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");
  a->SetObjectBList(objs);

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto a2 = ref->GetSharedObject();

  ASSERT_TRUE(a2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a, a2);
  EXPECT_EQ(a->GetValue(), "b1c1b2");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, SetObjectList) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local b1 = TestObjectB();\n"
                  "local c1 = TestObjectC();\n"
                  "local b2 = TestObjectB();\n"
                  "b1.SetValue(\"b1\");\n"
                  "c1.SetValue(\"c1\");\n"
                  "b2.SetValue(\"b2\");\n"
                  "a.SetObjectBList([b1, c1, b2]);\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto a2 = ref->GetSharedObject();

  ASSERT_TRUE(a2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a, a2);

  auto objs = a->GetObjectBList();
  ASSERT_EQ(objs.size(), 3);

  auto it = objs.begin();
  EXPECT_EQ((*it)->GetValue(), "b1");
  it++;
  EXPECT_EQ((*it)->GetValue(), "c1");
  it++;
  EXPECT_EQ((*it)->GetValue(), "b2");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, SetBadObjectList) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();
  engine.Using<objects::TestObjectD>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local b1 = TestObjectB();\n"
                  "local c1 = TestObjectC();\n"
                  "local d1 = TestObjectD();\n"
                  "b1.SetValue(\"b1\");\n"
                  "c1.SetValue(\"c1\");\n"
                  "d1.SetValue(1337);\n"
                  "a.SetObjectBList([b1, c1, d1, 3, \"a\"]);\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  auto messageLines = scriptMessages.Split("\n");
  messageLines.erase(messageLines.begin());

  EXPECT_EQ(messageLines.front(),
            "ERROR: SQUIRREL: AN ERROR HAS OCCURED "
            "[wrong type (TestObjectB expected, got TestObjectD)]");

  ASSERT_FALSE(ref);

  auto objs = a->GetObjectBList();
  ASSERT_EQ(objs.size(), 0);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, GetObjectListProp) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
    printf("%s", msg.C());
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local s = \"\";\n"
                  "\n"
                  "foreach(b in a.ObjectBList)\n"
                  "{\n"
                  "s += b.GetValue();\n"
                  "}\n"
                  "\n"
                  "a.SetValue(s);\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto b1 = std::make_shared<objects::TestObjectB>();
  b1->SetValue("b1");
  auto c1 = std::make_shared<objects::TestObjectC>();
  c1->SetValue("c1");
  auto b2 = std::make_shared<objects::TestObjectB>();
  b2->SetValue("b2");
  std::list<std::shared_ptr<objects::TestObjectB>> objs;
  objs.push_back(b1);
  objs.push_back(c1);
  objs.push_back(b2);
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");
  a->SetObjectBList(objs);

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto a2 = ref->GetSharedObject();

  ASSERT_TRUE(a2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a, a2);
  EXPECT_EQ(a->GetValue(), "b1c1b2");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, SetObjectListProp) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local b1 = TestObjectB();\n"
                  "local c1 = TestObjectC();\n"
                  "local b2 = TestObjectB();\n"
                  "b1.SetValue(\"b1\");\n"
                  "c1.SetValue(\"c1\");\n"
                  "b2.SetValue(\"b2\");\n"
                  "a.ObjectBList = [b1, c1, b2];\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto a2 = ref->GetSharedObject();

  ASSERT_TRUE(a2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(a, a2);

  auto objs = a->GetObjectBList();
  ASSERT_EQ(objs.size(), 3);

  auto it = objs.begin();
  EXPECT_EQ((*it)->GetValue(), "b1");
  it++;
  EXPECT_EQ((*it)->GetValue(), "c1");
  it++;
  EXPECT_EQ((*it)->GetValue(), "b2");

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, SetBadObjectListProp) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectA>();
  engine.Using<objects::TestObjectB>();
  engine.Using<objects::TestObjectC>();
  engine.Using<objects::TestObjectD>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(a)\n"
                  "{\n"
                  "local b1 = TestObjectB();\n"
                  "local c1 = TestObjectC();\n"
                  "local d1 = TestObjectD();\n"
                  "b1.SetValue(\"b1\");\n"
                  "c1.SetValue(\"c1\");\n"
                  "d1.SetValue(1337);\n"
                  "a.ObjectBList = [b1, c1, d1, 3, \"a\"];\n"
                  "return a;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectA>> ref;
  auto a = std::make_shared<objects::TestObjectA>();
  a->SetValue("testOf_");

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectA>>(a);

  auto messageLines = scriptMessages.Split("\n");
  messageLines.erase(messageLines.begin());

  EXPECT_EQ(messageLines.front(),
            "ERROR: SQUIRREL: AN ERROR HAS OCCURED "
            "[wrong type (TestObjectB expected, got TestObjectD)]");

  ASSERT_FALSE(ref);

  auto objs = a->GetObjectBList();
  ASSERT_EQ(objs.size(), 0);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, ListOfIntegers) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectE>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(e)\n"
                  "{\n"
                  "local sum = 0;\n"
                  "local intList = e.IntList;\n"
                  "\n"
                  "for(local i = 0; i < e.IntList.len(); ++i) {\n"
                  "sum += e.IntList[i];\n"
                  "}\n"
                  "\n"
                  "intList.append(sum);\n"
                  "e.IntList = intList;\n"
                  "\n"
                  "return e;\n"
                  "}\n"));

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectE>> ref;
  auto e = std::make_shared<objects::TestObjectE>();
  e->AppendIntList(1);
  e->AppendIntList(3);
  e->AppendIntList(3);
  e->AppendIntList(7);

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectE>>(e);

  EXPECT_EQ(scriptMessages, "");

  ASSERT_TRUE(ref);

  auto e2 = ref->GetSharedObject();

  ASSERT_TRUE(e2);

  EXPECT_EQ(scriptMessages, "");

  EXPECT_EQ(e, e2);
  ASSERT_EQ(e->IntListCount(), 5);
  EXPECT_EQ(e->GetIntList(0), 1);
  EXPECT_EQ(e->GetIntList(1), 3);
  EXPECT_EQ(e->GetIntList(2), 3);
  EXPECT_EQ(e->GetIntList(3), 7);
  EXPECT_EQ(e->GetIntList(4), (1 + 3 + 3 + 7));

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

TEST(ScriptEngine, Integer64) {
  String scriptMessages;

  Log::GetSingletonPtr()->AddLogHook([&scriptMessages](LogComponent_t comp,
                                                       Log::Level_t level,
                                                       const String& msg) {
    (void)comp;
    (void)level;

    scriptMessages += msg;
  });

  ScriptEngine engine;
  engine.Using<objects::TestObjectE>();

  EXPECT_TRUE(
      engine.Eval("function TestFunction(e)\n"
                  "{\n"
                  "print(typeof(e.Signed64));\n"
                  "print(typeof(e.Unsigned64));\n"
                  "e.Signed64 += 500;\n"
                  "e.Unsigned64 -= 1000;\n"
                  "return e;\n"
                  "}\n"));

  union {
    int64_t resultSigned;
    uint8_t randomDataSigned[8];
  };

  union {
    uint64_t resultUnsigned;
    uint8_t randomDataUnsigned[8];
  };

  for (int i = 0; i < 8; ++i) {
    randomDataSigned[i] = (uint8_t)libcomp::Crypto::GenerateSessionKey();
    randomDataUnsigned[i] = (uint8_t)libcomp::Crypto::GenerateSessionKey();
  }

  std::shared_ptr<Sqrat::ObjectReference<objects::TestObjectE>> ref;
  auto e = std::make_shared<objects::TestObjectE>();
  e->SetSigned64(resultSigned);
  e->SetUnsigned64(resultUnsigned);

  ref = Sqrat::RootTable(engine.GetVM())
            .GetFunction("TestFunction")
            .Evaluate<Sqrat::ObjectReference<objects::TestObjectE>>(e);

#ifdef SQRAT_WRAP_INTEGER64
  EXPECT_EQ(scriptMessages, "SQUIRREL: s64\nSQUIRREL: u64\n");
#else
  EXPECT_EQ(scriptMessages, "SQUIRREL: integer\nSQUIRREL: integer\n");
#endif

  ASSERT_TRUE(ref);

  auto e2 = ref->GetSharedObject();

  ASSERT_TRUE(e2);

#ifdef SQRAT_WRAP_INTEGER64
  EXPECT_EQ(scriptMessages, "SQUIRREL: s64\nSQUIRREL: u64\n");
#else
  EXPECT_EQ(scriptMessages, "SQUIRREL: integer\nSQUIRREL: integer\n");
#endif

  resultSigned += 500;
  resultUnsigned -= 1000;

  EXPECT_EQ(e, e2);
  EXPECT_EQ(e->GetSigned64(), resultSigned);
  EXPECT_EQ(e->GetUnsigned64(), resultUnsigned);

  scriptMessages.Clear();

  Log::GetSingletonPtr()->ClearHooks();
}

int main(int argc, char* argv[]) {
  try {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
  } catch (...) {
    return EXIT_FAILURE;
  }
}
