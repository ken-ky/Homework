#### 实验二：

##### 第一部分

1. 下载并安装相关文件

```sh
$ sudo apt-get install nginx	#安装nginx

# 安装mysql，设置密码
$ sudo apt-get install mysql-server
$ mysql> ALTER USER 'root'@'localhost' identified with mysql_native_password BY '123456';
$ mysql> flush privileges;

$ sudo apt-get install php-fpm php-mysqli php-gd php-mysql	#安装php等依赖配置
```

2. 进入`seed/var`文件夹，这个文件夹在`ls`命令以及GUI部分不能直接看到，因此我稍微查找了下其用途：[linux系统/var目录的作用 - 范仁义 - 博客园 (cnblogs.com)](https://www.cnblogs.com/Renyi-Fan/p/11504111.html)

```sh
$ sudo wget https://github.com/ethicalhack3r/DVWA/archive/master.zip --no-check-certificate
$ cd DVWA-master/config
$ sudo cp config.inc.php.dist config.inc.php	#重命名文件
$ gedit config.inc.php	#编辑数据库用户，密码
```

初始值：

![image-20240416222626473](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404162226609.png)

设置值：

![image-20240416222723876](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404162227984.png)

设置公钥和私钥：

![image-20240416223048252](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404162230406.png)

增加写权限：

```sh
$ sudo chmod +777 /var/www/html/DVWA-master/hackable/uploads/
$ sudo chmod +777 /var/www/html/DVWA-master/config
```

5. 修改`php-fpm`配置,将`unix socket`套接字改成`tcp/ip`方式，重新启动`php`

寻找配置文件目录：

```sh
$ pwd
/etc/php/7.4/fpm/pool.d
$ gedit /etc/php/7.4/fpm/pool.d
```

修改：

```cmake
;listen = /run/php/php7.2-fpm.sock #注释改行
listen = 127.0.0.1:9000  #增加ip端口接听
```

寻找`ini`文件

```sh
$ sudo find  /  -name php.ini
/etc/php/7.4/fpm/pool.d	#发现路径
$ gedit /etc/php/7.4/fpm/php.ini
```



6. 修改`nginx`设置

修改`/etc/nginx/nginx.conf`配置文件，http块中添加如下配置，重启`systemctl reload nginx.service`修改配置后使用`sudo nginx -t`进行验证

```php
server {
    listen 80 ;
    #listen [::]:80 default_server;

    root /var/www/html/DVWA-master;
    index index.php index.html index.htm index.nginx-debian.html;

    server_name dvwa;

    location / {
    	try_files $uri $uri/ /index.php$is_args$args =404;
    }

    location ~ \.php$ {
        # this is for the PHP part of DVWA to work properly
        fastcgi_split_path_info ^(.+\.php)(/.+)$;
        try_files $uri =404;
        fastcgi_pass 127.0.0.1:9000;
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME $request_filename;
        include fastcgi_params;
    }
}
```



![image-20240416232529219](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404162325316.png)

7. 访问`http://dvwa`：

![image-20240416233109596](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404162331706.png)

+ 在寻找无法执行`systemctl restart php7.4-fpm.service`原因时，我发现下图内容有时会导致无法开启`php`服务，但是之后重装`php`又可以了，其原因是：`#`不是注释语句，因此会被判断错误。

![image-20240417011522923](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404170115092.png)

创建数据库：

![image-20240417172859745](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404171728888.png)

![image-20240417173023230](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404171730330.png)



##### 第二部分

1. 使用上个实验的CA签名，配置数字证书：

```sh
$ openssl req -newkey rsa:2048 -sha256 -keyout server.key -out server.csr -subj "/CN=dvwa/C=US" -passout pass:kenky
$ cd ..
$ openssl ca -policy policy_anything -in server.csr -out server.crt -batch
```

![image-20240417180000217](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404171800380.png)

2. 修改`nginx`配置：

```php
server {
    listen 443 ssl;
    #listen [::]:80 default_server;

    root /var/www/html/DVWA-master;
    index index.php index.html index.htm index.nginx-debian.html;

    server_name dvwa;

    ssl_certificate     /home/seed/Public/server.crt;
    ssl_certificate_key  /home/seed/Public/server.key;

    location / {
    	try_files $uri $uri/ /index.php$is_args$args =404;
    }

    location ~ \.php$ {
        # this is for the PHP part of DVWA to work properly
        fastcgi_split_path_info ^(.+\.php)(/.+)$;
        try_files $uri =404;
        fastcgi_pass 127.0.0.1:9000;
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME $request_filename;
        include fastcgi_params;
    }
}
```

访问`https://dvwa/`：

![image-20240417181639528](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404171816661.png)



##### 第三部分

1. 对`http`抓包分析：

+ 找到TCP三次握手过程：

![image-20240418215541355](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404182155488.png)

+ 交换响应的报文：

![image-20240418215706854](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404182157924.png)

+ 找到`http`的流：

![image-20240418220036519](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404182200611.png)

+ 发现用户名、密码：

![image-20240418221834956](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404182218050.png)



2. 对`https`报文进行分析：

+ 首先配置TLS相关文件，使`Wireshark`能产生TLS的`debug`日志：

```sh
$ root@VM:/home/seed/# gedit .profile	#修改配置文件
```

这里需要注意，我们打开的`wireshark`是有`seed`与`root`两个用户之分的

![image-20240419213353946](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192133037.png)

改内容：

```php
export SSLKEYLOGFILE=/home/seed/Documents/wireshark_ssl.debug
```

可以在`wireshark`截取以下信息：

![image-20240419222809450](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192228624.png)

![image-20240419222915994](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192229109.png)



##### 第四部分：

1. 利用`burp suite`捕获`http`登录报文：

![image-20240419225700497](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192257614.png)

2. 利用`burp suite`捕获`https`登录`https://dvwa`报文：

![image-20240419231046221](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192310362.png)

3. 利用`burp suite`捕获`https`登录其它网站报文：

![image-20240419232506546](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202404192325699.png)

在qq邮箱进行扫码登录，发现`cookie`中含有qq号，说明登录成功；但是并没有返回密码，说明密码不以明文形式传输



