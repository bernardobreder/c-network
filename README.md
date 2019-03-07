# Objetivo

Foi construido 2 projetos nesse workspace

# Projeto 1

Desenho de um cliente servidor basico

# Projeto Raw-Package

Criação de uma comunicação de protocolo bruto.

Parece que no MAC o compartamento é diferente do linux.

Nessa etapa, como é mostrado abaixo, o cliente envia um pacote de 60 bytes, sendo que os 20 primeiros bytes são do header do pacote e os 40 bytes restantes são de dados.

```c
// client
// 45 00 00 28 41 41 00 00 40 FF EE EE 7F 00 00 01 7F 00 00 01 (1)
// 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
// 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
```

O servidor recebe um pacote de 60 bytes (3o bytes 0x3C) mas foi adicionado um pacote extra no inicio do pacote, descartando 20 bytes de dados. Isso faz com que seja reduzido a quantidade de dados para 40 bytes, por conta dos 20 bytes de pacote extra. Não entendi porque do pacote extra inicial, mas é interessante que o checksum do primeiro pacote é nulo e o segundo pacote é exatamento a requisição do cliente, podendo validar o checksum.

```c
// server
// 45 00 3C 00 1E F0 00 00 40 FF 00 00 7F 00 00 01 7F 00 00 01 (2)
// 45 00 00 28 41 41 00 00 40 FF EE EE 7F 00 00 01 7F 00 00 01 (1)
// 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
```

Por fim, o tcpdump (sudo tcpdump -i lo0 -X -vv) mostra a transferencia do pacote com o tamanho do primeiro pacote extra no servidor com alguns valores modificados. Mas mostra que não houve perda de dados

```c
// tcpdump
// 45 00 00 50 4c 4a 00 00 40 ff 00 00 7f 00 00 01 7f 00 00 01 (~2)
// 45 00 00 28 41 41 00 00 40 ff ee ee 7f 00 00 01 7f 00 00 01 (1)
// 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
// 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
```

# Referencia

- http://www.cs.princeton.edu/courses/archive/spr13/cos461/assignments-tcp.html
- https://gist.githubusercontent.com/austinmarton/1922600/raw/abab5894b8b33e12108dc7eec866e131c11aa0d6/sendRawEth.c
- https://sock-raw.org/papers/sock_raw
- http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
- https://www.tutorialspoint.com/unix_sockets/socket_core_functions.htm
- https://www.youtube.com/watch?v=BkQiZ5rSR8A
