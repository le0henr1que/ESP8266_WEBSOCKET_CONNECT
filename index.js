const express = require('express')
const path = require('path')
const { fileURLToPath } = require('url')



const app = express()
app.set('port', (process.env.PORT || 5000));
var server = app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});



const io = require('socket.io')(server, {
  cors: {
    origin: '*',
  }
})

app.use(express.static(path.join(__dirname, 'public')));
app.set('views', path.join(__dirname, 'public'));
app.engine('html', require('ejs').renderFile)
app.set('view engine', 'html')



app.get('/', (req, res) => {
    res.render('index.html')
})

let messagem = [];

let users = [];

//Conecta o socket
io.on('connection', socket => {


    users.push(socket.id)

    console.log(users)

    console.log(`Socket conectado: ${socket.id}`);

    //Carregando nmensagens no reload
    socket.emit('previusMessages', messagem)

    //Carregando no front a quantidade de usuarios
    socket.emit('users', users)

    //Eitindo para o front as a quantidade de usuario
    socket.broadcast.emit('recivedUsers', users)


      //Enviando as mensagens para o front
      socket.on('sendMessage', data => {
        //Colocando os a dados da mensagem no front
        messagem.push(data)
        socket.broadcast.emit('receivedMessage', data)
      });


 
    // Gatilho para capturar a desconexão
    socket.on('disconnect', function(data){
      console.log('user ' + data + ' disconnected');

    // Retirando do Array user disconectado
      users.splice(users.indexOf(socket.id), 1);

    // Enviar para o Front se o usuario desconectar
      socket.emit('usersLogOut', users)
      socket.broadcast.emit('recivedUsersLogOut', users)

      

      console.log(data)

    });
  });