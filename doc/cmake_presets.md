# CMake 预设

本节学习 CMake 预设。

- [CMake 预设](#cmake-预设)
  - [0. 学习目标](#0-学习目标)
  - [1. 为什么需要预设？](#1-为什么需要预设)
  - [2. 使用 CMake 预设需要满足什么条件？](#2-使用-cmake-预设需要满足什么条件)
  - [3. 如何在项目中使用 CMake 预设？](#3-如何在项目中使用-cmake-预设)
  - [4. CMake 预设语法详述？](#4-cmake-预设语法详述)
    - [4.0. 一些需要提前说明的概念](#40-一些需要提前说明的概念)
      - [4.0.1. condition（条件）](#401-condition条件)
    - [4.1. include 字段](#41-include-字段)
    - [4.2. configurePresets 字段](#42-configurepresets-字段)

## 0. 学习目标
1. 为什么需要预设？
2. 使用 CMake 预设需要满足什么条件？
3. 如何在项目中使用 CMake 预设？
4. CMake 预设语法详述？

## 1. 为什么需要预设？ 
首先来说说什么是 CMake 预设？随着我们对 CMake 的使用的熟练，很多使用场景开始变得重复而烦躁，比如配置阶段，每次配置都要敲很多 cmake 参数，比如 -S -B -G -D 等等。CMake 的开发人员估计也是注意到了这一点，所以在 CMake 3.19 中加入了 CMake 预设。

CMake 预设的出现就是为了解决一些重复且相对固定的 CMake 使用场景（本节后面会详述）。

需要说明一点，CMake 3.19 版本中加入的预设支持的功能还很初级，所以要使用相对完善的 CMake 预设请至少使用 CMake 3.24 版本，之后随着 CMake 版本的更新，预设的支持也会得到进一步的提升。

## 2. 使用 CMake 预设需要满足什么条件？
如果你打算使用 CMake 预设，CMake 至少需要 3.19 的版本，因为预设的概念是 3.19 版本引入的。但是 3.19 版本中的预设支持还很初级，只支持配置阶段的预设。如果要使用较为完整的预设支持，请使用其他高版本，下面我将列出其他高版本中分别加入了哪些 CMake 预设支持。

- 3.19
  - 引入 CMake 配置阶段预设支持
  - configurePresets
  - "version": 1
- 3.20
  - 引入 CMake 构建阶段和测试阶段预设支持
  - buildPresets
  - testPresets
  - "version": 2
- 3.21
  - 支持预设中使用条件判断以及去除之前的版本必须指定生成器和构建目录的限制。
  - "version": 3
- 3.23
  - 支持预设中使用 include 包含其他预设文件
  - "version": 4
- 3.24
  - 改动较小，添加一些字段支持
  - "version": 5
- 3.25
  - 引入 CMake 打包阶段预设支持
  - 引入 CMake 工作流预设支持
  - packagePresets
  - workflowPresets
  - "version": 6

通过以上列出的各个 CMake 版本预设的支持，我的建议是使用最新的 CMake 发布版本。

## 3. 如何在项目中使用 CMake 预设？
CMake 预设使用 json 文件来表述，对于一个 CMake 构建的项目，如果要使用 CMake 预设，需要在项目的顶级目录添加一个名为 CMakePresets.json 的文件，然后在这个 json 文件中去描述预设。

CMakePresets.json 文件因为是项目的一部分，所以需要和其他项目文件一样加入版本控制中。同时要确保 CMakePresets.json 中不要包含硬编码路径。

当然为了满足开发人员在本地使用的方便，CMake 支持开发人员自己在项目顶级目录中添加一个名为CMakeUserPresets.json 的文件，该文件属于开发人员自己的文件，而不属于某个项目，所以不需要也不要提交到版本管理中。当然在 CMakeUserPresets.json 文件中，开发人员可以随便硬编码路径，因为都是在自己本机使用，不会共享给其他人使用。

如果使用 git 作为版本控制系统，一个较好的做法是把 CMakeUserPresets.json 文件加入到 .gitignore 中。其他版本控制系统应该也有类似的机制。

下面看一个来自 CMake 官方文档的 CMakePresets.json 文件示例，其中包含了 CMake 预设的绝大多数语法。
```json
{
  "version": 6,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 23,
    "patch": 0
  },
  "include": [
    "otherThings.json",
    "moreThings.json"
  ],
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Default Config",
      "description": "Default build using Ninja generator",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/default",
      "cacheVariables": {
        "FIRST_CACHE_VARIABLE": {
          "type": "BOOL",
          "value": "OFF"
        },
        "SECOND_CACHE_VARIABLE": "ON"
      },
      "environment": {
        "MY_ENVIRONMENT_VARIABLE": "Test",
        "PATH": "$env{HOME}/ninja/bin:$penv{PATH}"
      },
      "vendor": {
        "example.com/ExampleIDE/1.0": {
          "autoFormat": true
        }
      }
    },
    {
      "name": "ninja-multi",
      "inherits": "default",
      "displayName": "Ninja Multi-Config",
      "description": "Default build using Ninja Multi-Config generator",
      "generator": "Ninja Multi-Config"
    },
    {
      "name": "windows-only",
      "inherits": "default",
      "displayName": "Windows-only configuration",
      "description": "This build is only available on Windows",
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Windows"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "default",
      "configurePreset": "default"
    }
  ],
  "testPresets": [
    {
      "name": "default",
      "configurePreset": "default",
      "output": {"outputOnFailure": true},
      "execution": {"noTestsAction": "error", "stopOnFailure": true}
    }
  ],
  "packagePresets": [
    {
      "name": "default",
      "configurePreset": "default",
      "generators": [
        "TGZ"
      ]
    }
  ],
  "workflowPresets": [
    {
      "name": "default",
      "steps": [
        {
          "type": "configure",
          "name": "default"
        },
        {
          "type": "build",
          "name": "default"
        },
        {
          "type": "test",
          "name": "default"
        },
        {
          "type": "package",
          "name": "default"
        }
      ]
    }
  ],
  "vendor": {
    "example.com/ExampleIDE/1.0": {
      "autoFormat": false
    }
  }
}
```

## 4. CMake 预设语法详述？
看完上面的 CMakePresets.json 例子，是不是感觉很多陌生的字段名字，不知道什么意思，不要着急，听我慢慢道来。

先看一个骨架，不要被细节打败。
```json
{
  "version": 6,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 24,
    "patch": 0
  },
  "include": [
  ],
  "configurePresets": [
  ],
  "buildPresets": [
  ],
  "testPresets": [
  ],
  "packagePresets": [
  ],
  "workflowPresets": [
  ],
  "vendor": {
  }
}
```
只看这个 json 是不是清晰多了，为了描述 CMake 预设，我们只需要在 CMakePresets.json 按照 json 语法写上一些 CMake 预设必须的字段和可选字段即可。

- "version" 字段（必须），制定 CMake 预设的版本，注意不要和 CMake 的版本搞混淆了。比如上例中，CMake 预设的版本被指定为 6，CMake 预设版本 6 是在 CMake 3.25 中引入的。
- "cmakeMinimumRequired" 字段（必须），用于指定 CMake 的最小版本，这个需要和项目中 CMakeLists.txt 中 cmake_minimum_required(VERSION 3.24 FATAL_ERROR) 指定的版本一致，形式固定，按照上例中指定即可，上例中指定 CMake 的最小版本为 3.24.0。
- "include" 字段（可选），用于包含其他 json 文件。
- "configurePresets" 字段（可选），用于描述配置阶段。
- "buildPresets" 字段（可选），用于描述构建阶段。
- "testPresets" 字段（可选），用于描述测试阶段。
- "packagePresets" 字段（可选），用于描述打包阶段。
- "workflowPresets" 字段（可选），用于描述 CMake 工作流。
- "vendor" 字段（可选），用于描述供应商的特定信息。

### 4.0. 一些需要提前说明的概念

为了更好的理解 CMake 各个阶段的预设，我把各个阶段预设需要用到的一些概念先放到这里说明一下：

#### 4.0.1. condition（条件）
  - CMake 预设 3 版本引入，用于确定是否启用预设。
  - condition 的值可以是一个 boolean 值、null 或者对象。
  - 如果值为 null 表示启用该预设，但是不会被继承。
  - 如果值为对象，则由子条件（not、anyOf、allOf）
    - type
      - "const" : 表示条件不变
        - value 字段（必须），提供常量值，能转化为 boolean 值。
      - "equals"
      - "notEquals"
        - lhs
        - rhs
      - "inList"
      - "notInList"
        - string
        - list
      - "matches"
      - "notMatches"
        - string
        - regex
      - "anyOf"
      - "allOf"
        - conditions
      - "not"
        - condition

- 例子
```json
{
  "condition": {
    "type": "equals",
    "lhs": "${hostSystemName}",
    "rhs": "Windows"
  }
}
```

### 4.1. include 字段
在 CMake 预设中，include 用于包含其他 json 文件，如果所有的预设信息都在 CMakePresets.json 一个 json 文件中描述，那随着项目的复杂度增加，预设需求的增加，CMakePresets.json 势必会变得非常的臃肿，所以 CMake 预设 4 版本（CMake 3.23 引入）中添加 include 字段支持，用于包含特定功能的 CMake 预设 json 描述文件。

默认情况下，如果开发者在自己的项目中使用了 CMakeUserPresets.json 描述一些自己环境特定的预设信息，那么 CMakeUserPresets.json 默认 include 了 CMakePresets.json。

一开始的例子中，有下面这样一段：
```json
{
  "include": [
    "otherThings.json",
    "moreThings.json"
  ]
}
```
一个 CMake 预设 json 文件可以被 include 多次，但是不允许循环包含。

### 4.2. configurePresets 字段
```json
{
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Default Config",
      "description": "Default build using Ninja generator",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/default",
      "cacheVariables": {
        "FIRST_CACHE_VARIABLE": {
          "type": "BOOL",
          "value": "OFF"
        },
        "SECOND_CACHE_VARIABLE": "ON"
      },
      "environment": {
        "MY_ENVIRONMENT_VARIABLE": "Test",
        "PATH": "$env{HOME}/ninja/bin:$penv{PATH}"
      },
      "vendor": {
        "example.com/ExampleIDE/1.0": {
          "autoFormat": true
        }
      }
    },
    {
      "name": "ninja-multi",
      "inherits": "default",
      "displayName": "Ninja Multi-Config",
      "description": "Default build using Ninja Multi-Config generator",
      "generator": "Ninja Multi-Config"
    },
    {
      "name": "windows-only",
      "inherits": "default",
      "displayName": "Windows-only configuration",
      "description": "This build is only available on Windows",
      "condition": {
        "type": "equals",
        "lhs": "${hostSystemName}",
        "rhs": "Windows"
      }
    }
  ]
}
```
从上面的例子中可以看出 configurePresets 字段的值是一个数组，数组里面包含了多个对象，每个对象描述一个 CMake 配置阶段的预设。比如上例中一共有 3 个配置阶段预设。

- name 字段（必须），标识名字。
- displayName 字段（可选），一个人性化的名字，比如 name 字段的值不太好理解的时候就可以增加该字段，然后使用一个字符串描述 name 字段的值的含义。
- description 字段（可选），如果该预设需要进一步的解释，可以增加该字段描述。
- hidden 字段（可选），如果一个预设只想作为基础预设，供其他预设继承，那可以把它标记为 hidden。
  - hidden 的预设不需要指定 generator、binaryDir。
  - cmake --preset= 命令不能指定 hidden 的预设名字。
  - CMake GUI 对 hidden 的预设不可见。
- inherits 字段（可选），用于从其他预设继承描述信息。
  - 可以是字符串，也可以是字符串数组。
  - 默认继承除了 name、hidden、inherits、description 和 displayName 以外的其他描述信息，由新的预设重写这些字段的值。
  - 如果使用字符串数组从多个预设继承了相同的字段，那么字符串数组中首先出现的预设中的字段优先使用。
  - 只能继承同一个文件中的预设或者 include 引入的文件中的预设。
  - CMakePresets.json 不能继承 CMakeUserPresets.json 中的预设。
- condition 字段（可选），详见[condition](#401-condition条件)。
- vendor 字段（可选），同顶级 vendor 字段。
- generator 字段（可选），指定生成器。
  - 对于 Visual Studio 生成器，和 -G 命令行参数不一样，不能直接指定平台，需要使用 architecture 字段代替。
- architecture, toolset 两个字段（可选），用于指定生成器支持的平台和工具集
  - architecture 同 cmake -A
  - toolset 同 cmake -T
  - 这两个字段的值可以是一个字符串或者一个对象，如果是一个对象，还需要如下字段描述：
    - value 字段（可选）
    - strategy 字段（可选），告诉 CMake 如何处理 architecture 或者 toolset。
      - "set"，设置相应的值，对于不支持相应字段的生成器会导致错误。
      - "external"，即使生成器支持也不设置改值，这很有用。比如使用 ninja 生成器，IDE 知道如何从 architecture 或者 toolset 设置 C++ 环境，这种情况下，CMake 将忽略该字段，但是 IDE 可以在调用 CMake 之前使用它们来设置环境。
- toolchainFile 字段（可选），指定工具链文件路径，支持宏扩展，其值优先于 CMAKE_TOOLCHAIN_FILE 变量的值。
- binaryDir 字段（可选），指定二进制输出目录，支持宏扩展，同 cmake -B 参数。
- installDir 字段（可选），指定安装目录，支持宏扩展。
- cmakeExecutable 字段（可选），表示用于此预设的 CMake 可执行文件的路径，保留给 IDE 使用，CMake 自身不使用。
- cacheVariables 字段（可选），用于指定缓存变量，name 不能为空，值可以是 boolean 值，null，字符串或者对象。如果是对象，包含如下字段：
  - type
  - value
- environment 字段（可选），用于指定环境变量。名字不能为空，值可以是 null，字符串。
- warnings 字段（可选），用于指定要启用的警告级别日志。
  - dev，等同于 -Wdev 或者 -Wno-dev
  - deprecated, 等同于 -Wdeprecated 或者 -Wno-deprecated
  - uninitialized， 等同于 --warn-uninitialized
  - unusedCli， 等同于 --no-warn-unused-cli
  - systemVars， 等同于 --check-system-vars
- errors 字段（可选），用于指定要启用的错误级别日志
  - dev
  - deprecated
- debug 字段（可选），用于指定要启用的 debug 级别的日志
  - output，等同于 --debug-output
  - tryCompile，等同于 --debug-trycompile
  - find，等同于 --debug-find