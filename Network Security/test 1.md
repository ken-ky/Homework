### 实验1—应用密码学

#### 任务一：编译`Openssl 3.x`版本，运行test通过

(1) 安装编译依赖包：

```sh
$ apt update
$ apt install gcc make  zlib1g-dev  libpam0g-dev libkrb5-dev libedit-dev -y
```

(2) 克隆 `openssl`项目：

```sh
$ git clone https://github.com/openssl/openssl.git openssl-3.2
```

(3) 进入项目执行并指定安装路径：

```sh
LDFLAGS="-Wl,-rpath -Wl,/opt/openssl/lib64" ./config shared --prefix="/opt/openssl"
```

(4) 执行make命令进行编译，然后使用make install命令进行安装：

```sh
$ sudo make 
$ sudo make install
```

(5) 查看安装的`openssl`版本，运行命令：

```sh
$ /opt/openssl/bin/openssl --version
```

![image-20240325200643063](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403252006176.png)



---



#### 任务二：使用`Openssl` RSA和ECDSA两种算法，分别生成RSA和ECDSA证书密钥对

##### 1. 为`modelCA`生成自签名根证书

(1) 在当前工作目录下创建目录`demoCA`，执行

```sh
$ mkdir demoCA
$ cd demoCA
$ mkdir certs crl newcerts private
$ touch index.txt serial
$ echo 1000 > serial	# 将1000写入serial文件中
```

+ `touch`命令：这条命令使用 `touch` 创建了两个空文件，一个是 `index.txt`，另一个是 `serial`。如果这两个文件不存在，`touch` 命令会创建它们；如果已经存在，则会更新它们的访问和修改时间。
+ `echo 1000 > serial`：这条命令将数字 `1000` 写入了 `serial` 文件中。这通常用于生成证书颁发机构 (CA) 的序列号文件，在 CA 签发证书时，会根据该序列号进行递增操作，以确保每个签发的证书具有唯一的标识序号。

(2) 生成根证书私钥

```sh
$ /opt/openssl/bin/openssl genrsa -des3 -out ./demoCA/private/cakey.pem 4096
```

(3) 生成自签名根证书

```sh
$ /opt/openssl/bin/openssl req -x509  -days 3650 -key ./demoCA/private/cakey.pem -out ./demoCA/cacert.pem -subj "/CN=www.modelCA.com/O=Model CA LTD./C=US"
```



+ 调用`private`中的密钥：

![image-20240325222859941](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403252229016.png)

+ 调用外部证书：

![image-20240325223108308](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403252231422.png)



##### 2. 生成RSA加密证书

(1) 生成RSA私钥`server.key`及证书请求文件`server.csr`

```sh
$ /opt/openssl/bin/openssl req -newkey rsa:2048 -sha256 -keyout server.key -out server.csr -subj "/CN=www.bank32.com/O=Bank32 Inc./C=US" -passout pass:dees
```

+ 值得注意的字段：

  + `req`：这个命令告诉 OpenSSL 工具执行证书请求相关的操作。

  + `-newkey rsa:2048`：这部分指定了要创建一个新的 RSA 密钥对，其中 2048 是密钥的位数。

  + `-sha256`：这指定了在证书请求中要使用 SHA-256 哈希算法。

  + `-keyout server.key`：这表示生成的私钥将保存在` server.key` 文件中。
    + `-subj "/CN=www.bank32.com/O=Bank32 Inc./C=US"`：这是证书主题字段，包含了一些关键的信息：
      - `/CN=`: Common Name，通常是服务器的域名。
      - `/O=`: Organization，组织名称。
      - `/C=`: Country，国家/地区。

  + `-passout pass:dees`：这部分设置了私钥的输出密码为 `"dees"`（可改，我的是`kenky`）。

(2) 向`modelC`请求签名生成证书

```sh
$ /opt/openssl/bin/openssl ca -policy policy_anything -in server.csr -out server.crt -batch 
```

+ 值得注意的字段：
  - `ca`：这个命令告诉 OpenSSL 工具执行证书颁发机构 (CA) 相关的操作。
  - `-policy policy_anything`：这部分指定了使用名为 `policy_anything` 的策略来验证签名请求。`policy_anything` 可能是一个自定义的策略名称。
  - `-batch`：这个选项告诉 OpenSSL 在运行时不要等待用户输入，而是使用默认值或预先设置好的选项来自动处理证书签发过程，适用于批量处理多个证书请求的情况。

![image-20240325224541629](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403252245718.png)

(3) 验证证书签名

```sh
$ /opt/openssl/bin/openssl verify  -partial_chain -CAfile ./demoCA/cacert.pem  -verbose server.crt
```

+ 值得注意的字段：
  + `verify`：这个命令告诉 OpenSSL 工具执行证书验证相关的操作。
  + `-partial_chain`：这个选项告诉 OpenSSL 在验证证书时，允许证书链中不包含完整的证书链。通常情况下，如果要验证的证书只包含部分证书链，可以使用这个选项。
  + `-CAfile ./demoCA/cacert.pem`：这表示使用 `cacert.pem` 文件中的证书作为根证书颁发机构 (CA) 来验证待验证的证书。指定 `-CAfile` 选项是为了告诉 OpenSSL 使用特定的 CA 证书文件进行验证。
  + `-verbose`：这个选项告诉 OpenSSL 输出更详细的信息，包括证书的详细验证过程和结果。



##### 3. 生成ECDSA签名证书

(1) 生成`ecdsa`私钥

```sh
$ /opt/openssl/bin/openssl ecparam -name prime256v1 -genkey -out ecdsa.key
```

+ 值得注意的字段：
  - `ecparam`：这个命令告诉 OpenSSL 工具执行椭圆曲线相关操作。
  - `-name prime256v1`：这部分指定了要使用的椭圆曲线的名称，这里是 `prime256v1`，也称为 `secp256r1`，它是一种常用的椭圆曲线参数，适用于 ECDSA 签名算法。
  - `-genkey`：这个选项告诉 OpenSSL 生成一个新的密钥对。

(2) 生成证书请求文件

```sh
$ /opt/openssl/bin/openssl req -new -key ecdsa.key -out ecdsa.csr -subj "/CN=www.bank32.com/O=Bank32 Inc./C=US" -passout pass:dees
```

(3) 向`modelCA`请求签名生成数字证书

```sh
$ /opt/openssl/bin/openssl ca -policy policy_anything -in ecdsa.csr -out ecdsa.crt -batch
```

这里会发生一个错误：

![image-20240325230749466](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403252307534.png)

这意味着我们使用的证书内容发生了重复

+ 解决办法：使用不同的签名地址

  ```sh
  $ /opt/openssl/bin/openssl req -new -key ecdsa.key -out ecdsa.csr -subj "/CN=www.bank31.com/O=Bank32 Inc./C=US" -passout pass:dees
  $ /opt/openssl/bin/openssl ca -policy policy_anything -in ecdsa.csr -out ecdsa.crt -batch
  ```

  ![image-20240326133429224](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261334411.png)

  这里需要注意一个问题，我们使用的变成了针对`"/CN=www.bank31.com/O=Bank32 Inc./C=US" -passout pass:dees`的签名

(4) 验证数字签名

```sh
$ /opt/openssl/bin/openssl verify  -partial_chain -CAfile ./demoCA/cacert.pem  -verbose ecdsa.crt
```

![image-20240326133613504](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261336576.png)



---



#### 任务三：使用SM2双证书模拟文件加密传输

##### 1. 客户端A与服务端B执行TLS握手协议建立TLS连接

(1) 客户端A对服务端B发来的证书进行验证

```sh
$ /opt/openssl/bin/openssl verify  -partial_chain -CAfile ./demoCA/cacert.pem  -verbose ecdsa.crt	# 对生成的ECDSA证书进行验证
$ /opt/openssl/bin/openssl verify  -partial_chain -CAfile ./demoCA/cacert.pem  -verbose server.crt	# 对生成的server证书进行验证
```

若验证通过，客户端A向服务器端B发送客户端密钥交换`(Client Key Exchange)`消息。发送一个使用服务器加密证书公钥加密的**随机生成的预主密钥**。我们手动创建一个密钥文件来模拟。

【接下来干这事情：创建一个密钥文件】

(2) 客户端A生成预主密钥

```sh
$ /opt/openssl/bin/openssl rand -out key 32 
```

![image-20240326134901525](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261349637.png)

可以看到生成了`key`文件为御主密钥

(3) 客户端A提取客户端B的加密证书中公钥

```sh
$ /opt/openssl/bin/openssl x509 -in server.crt -pubkey -noout >rsa_public.key
```

上面命令的意思就是从`server.crt`得到公钥放在`rsa_public.key`中

![image-20240326135216839](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261352909.png)

(4) 客户端A使用加密公钥加密预主密钥

```sh
$ /opt/openssl/bin/openssl pkeyutl -encrypt -pubin -inkey rsa_public.key -in key -out enckey	# 进行加密操作
```

服务端B收到后**使用加密私钥进行解密**，之后**客户端A、服务器端B使用预主密钥来生成会话密钥**。简略操作，我们直接将发送的预主密钥当作会话密钥使用。

(5) **客户端B**使用加密私钥解密预主密钥

```sh
$ /opt/openssl/bin/openssl pkeyutl -decrypt -inkey server.key -in enckey -out deckey
```

![image-20240326135812736](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261358852.png)

<font color='red'>注意：虽然这些文件都是在同一文件夹，但是其实是不同的主机端！</font>【当然，开两台虚拟机进行交换也是可以的】

之后客户端A与服务器端B之间发送更改密码规范`(Change Cipher Spec)`消息，告知进一步的通信将被加密。最后互相发送一个加密的完成`(Finished)`消息，其中双方验证之前交换的信息，验证失败握手协议将失败，TLS连接中断。验证成功则完成TLS握手协议。

+ `deckey`最后是这个样子的：

![image-20240326140540755](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261405864.png)



##### 2. TLS数据传输

客户端A和服务器B端完成握手协议后，他们可以开始交换数据。

由于非对称加密算法的速度问题，数据加密**采用对称加密算法**。

服务器端B使用TLS握手阶段商议选择好的密钥套件（这里我们使用AES/SM4对称加密算法）、加密工作模式及会话密钥加密客户端A请求的文件。

```sh
$ touch req_file	# 创建一个req_file文件
$ echo “hello TLS” > req_file	# 将传输信息写入
```

(6) 服务器端B使用约定会话密钥加密

```sh
$ /opt/openssl/bin/openssl enc -e -aes-256-cbc -in req_file -out enc_file -pass file:deckey
```

![image-20240326143748083](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261437161.png)

上面的消息注明了我们目前指定的算法已经不安全了，可以指定`pbkdf2`进行代替

以下命令会告诉我们可使用哪些算法：

```sh
$ /opt/openssl/bin/openssl	# 查看可使用算法
```

(7) 服务器端TLS协议使用签名私钥对文件签名

```sh
$ openssl pkeyutl -sign -in enc_file -inkey ecdsa.key -out file_sig
```

生成新的签名文件`file_sig`

客户端A接受到文件后，对文件使用服务端B签名公钥验签检查其完整性，并使用会话密钥解密。

(8) 服务端A提取签名证书公钥

```sh
$ /opt/openssl/bin/openssl x509 -in ecdsa.crt -pubkey -noout >ecdsa_public.key
```

(9) 使用签名公钥验签，若签名失败则表明数据完整性被破坏

```sh
$ /opt/openssl/bin/openssl pkeyutl -verify -pubin -in [plaintext] -inkey ecdsa_public.key  -sigfile file_sig  -rawin	# plaintext为之前签名私钥
```

![image-20240326145555527](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261455598.png)

发现这里验证数据完整性失败了

但是，假如说使用下面的命令就不会发生上述错误：

```sh
$ /opt/openssl/bin/openssl pkeyutl -verify -pubin -in [plaintext] -inkey ecdsa_public.key  -sigfile file_sig	# plaintext为之前签名私钥
```

![image-20240326173252399](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261732633.png)



(10) 使用会话密钥解密文件

```sh
$ /opt/openssl/bin/openssl enc -d -aes-256-cbc -in enc_file -out dec_file  -pass file:key
```

![image-20240326150109984](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261501084.png)

![image-20240326145801334](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261458443.png)

不过这里的解密倒是成功了



#### 任务四：测试后量子算法标准`CRYSTALS-Kyber`,对比和RSA、ECDSA在性能上的不同 （附加实验）

##### 1. 安装`liboqs`

安装依赖项：

```sh
$ sudo apt install astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind
```

使用以下代码进行安装：

```sh
$ git clone https://github.com/open-quantum-safe/liboqs
$ cd liboqs
$ mkdir build
$ cmake -GNinja	# 这里可以在build文件中进行custom定制
$ ninja
```

+ 具体的相关参数可以查看`git-hub`地址：

  [GitHub - open-quantum-safe/liboqs：用于抗量子加密的原型设计和实验的 C 库](https://github.com/open-quantum-safe/liboqs)

这里先使用测试套件：

```sh
$ ninja run_tests
```

![image-20240326161836106](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261618235.png)

##### 2.  安装及启动`oqs-provider`

下载并安装项目：

```sh
$ git clone https://github.com/open-quantum-safe/oqs-provider
```

构建并测试：

```sh
$ scripts/fullbuild.shscripts/runtests.sh
```

![image-20240326164350821](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202403261643890.png)

【不大会用`make`，之后可能会尝试】



---



#### 五、实验结论及思考题

##### （1）`X.509`数字证书包含哪些内容

+ 版本号（Version）：指定证书所采用的 X.509 标准版本，通常为 v1, v2, v3。
+ 序列号（Serial Number）：唯一标识证书的序列号。
+ 签发者（Issuer）：证书颁发机构（CA）的信息，如名称、组织等。
+ 有效期（Validity）：证书的生效日期和截止日期。
+ 主体（Subject）：证书持有者（通常是公钥持有者）的信息，如名称、组织等。
+ 公钥（Public Key）：证书持有者的公钥，用于加密通信和数字签名验证。
+ 数字签名算法（Signature Algorithm）：用于生成数字签名的算法，如 RSA、DSA、ECDSA 等。
+ 证书扩展（Extensions）：包含各种扩展信息，如密钥用途、密钥标识、证书策略等。
+ 颁发者数字签名（Issuer Signature）：CA 对证书内容的数字签名，用于验证证书的真实性和完整性。



##### （2）SSL握手期间，浏览器会做两个重要的验证。一个是核对接收到的证书是否有效。第二个验证是什么？这两个验证的目的分别是什么？

+ 验证证书是否有效：确保通信双方的身份信息是真实可信的，避免伪造证书或未经授权的证书引起的安全问题。
+ 验证服务器的身份：确保用户与正确的服务器建立安全连接，防止中间人攻击或欺诈等风险。通过验证服务器的身份，用户可以确认他们正在与目标服务器通信，而不是恶意攻击者。



##### （3）有哪些对公钥基础设施上实施攻击的案例

1. **伪造证书（Certificate Forgery）**：攻击者可能尝试伪造有效的数字证书，冒充合法实体的身份，从而欺骗用户或系统相信他们是合法的通信方。
2. **中间人攻击（Man-in-the-Middle Attack）**：攻击者试图插入自己在通信双方之间，窃取通信内容或篡改通信数据。这种攻击可能会侵入到 PKI 的信任链中，例如通过伪造证书或劫持域名进行攻击。
3. **私钥泄露（Private Key Compromise）**：私钥是数字证书的核心部分，如果私钥泄露给了攻击者，他们就能够解密加密的通信内容或冒充证书持有者进行欺诈。
4. **拒绝服务攻击（Denial of Service, DoS）**：攻击者可能试图破坏 PKI 的正常运行，例如通过攻击证书颁发机构的服务、抢占域名或者干扰证书验证流程来影响通信的正常进行。
5. **社会工程和钓鱼攻击（Social Engineering and Phishing）**：攻击者可能试图通过欺骗手段获取用户的私钥、证书或其他敏感信息，从而进一步渗透 PKI 系统。

##### （4）HTTPS可以防御中间人攻击，但是可以安装HTTPS代理来监视和修改HTTPS流量。代理基本上可以看作“中间人”。这是否意味着HTTPS仍然会受到中间人的攻击呢？请解释。

+ 虽然 HTTPS 可以有效防御传统的中间人攻击，但是当使用 HTTPS 代理时，仍然存在一定程度上受到中间人攻击的风险。
+ 仍然可能受到中间人攻击的原因：
  + **信任链的问题**：当用户安装根证书或信任特定的证书颁发机构（CA）时，代理可能会使用这些证书来签发伪造的证书，从而欺骗用户认为代理是合法的服务器。
  + **证书校验的绕过**：一些代理可能会使用自签名证书或无效证书来与客户端建立连接，此时客户端如果没有进行严格的证书校验，就容易受到攻击。
  + **代理的恶意行为**：有些恶意的 HTTPS 代理可能会故意篡改传输的数据，插入广告、恶意代码或窃取用户的敏感信息。