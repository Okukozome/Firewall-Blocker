[English Version](#Firewall-Blocker) | [中文版](#一键配置防火墙阻止策略)

# 一键配置防火墙阻止策略

## 简介

本程序可以自动查找所在目录（及子目录）下所有的 `.exe` 可执行文件，一次性配置所有的出入站阻止规则，从而禁止特定软件联网。  

> **注意：** 如果电脑正在开启 VPN 的系统代理，则配置好的防火墙阻止策略会暂时失效。手动配置的结果也是一样的，限于个人能力无法解决。

## 使用方法

将本程序复制到 **想要阻止联网的软件的安装目录下**，然后双击运行。

> 你可以对目录下查找到的每个程序，逐个决定是否配置禁止联网。没有特殊要求的话一路点击确定即可。

> 如果提示查找到的 `.exe` 文件过多，有可能你将程序复制到了错误的目录下（例如桌面或 `C:\Program Files\`），如果选择继续会导致目录下所有的应用被禁止联网，请谨慎操作。

-----

# Firewall-Blocker

## Introduction

This program automatically searches for all `.exe` executable files in the current directory and its subdirectories, and configures inbound and outbound blocking rules for all of them in one go, thereby preventing specific software from accessing the internet.

> **Note:** If your computer is using a VPN system proxy, the configured firewall blocking rules may be temporarily ineffective. This is also true for manually configured rules, and due to personal limitations, I can't resolve this issue.

## Usage

Copy this program to the **installation directory of the software you want to block from accessing the internet**, and then double-click to run it.

> You can choose whether to configure internet blocking for each found program individually. If there are no special requirements, you can simply click "OK" all the way through.

> If there is a warning that too many `.exe` files have been found, it is possible that you have copied the program to the wrong directory (such as the Desktop or `C:\Program Files\`). Proceeding may block all applications in the directory from accessing the internet, so please act with caution.
