using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using static System.Net.Mime.MediaTypeNames;

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public const uint CREATE_ROOM_CODE = 108;
        public const uint GET_PLAYERS_IN_ROOM_CODE = 104;
        public const uint GET_ROOMS_CODE = 103;
        public CreateRoomWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;

        private void BackToLastWindow(object sender, RoutedEventArgs e)
        {
            MainWindow objMainWindow = new MainWindow(m_client, m_username);
            this.Visibility = Visibility.Hidden;
            objMainWindow.Show();
        }

        private void CreateRoomClick(object sender, RoutedEventArgs e)
        {
            var crJsonObject = new JObject
            {
                { "name", roomName.Text},
                { "maxPlayers", int.Parse(maxPlayers.Text) },
                { "numOfQuestionsInGame", 10 },
                { "timePerQuestion", int.Parse(timePerQuestion.Text) },
            };

            byte[] data = LoginWindow.serializeMessage(crJsonObject, CREATE_ROOM_CODE);
            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                WaitingRoomWindow objWaitingRoomWindow = new WaitingRoomWindow(m_client, m_username);
                JObject emptyJson = new JObject();

                data = LoginWindow.serializeMessage(emptyJson, GET_ROOMS_CODE);
                stream.Write(data, 0, data.Length);

                buffer = new byte[1024];
                bytesRead = stream.Read(buffer, 0, buffer.Length);
                response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                serverMSG = response.Substring(5);

                joRecive = JObject.Parse(serverMSG);
                if (joRecive.ContainsKey("status"))
                {
                    //JArray joArrRooms = (JArray)joRecive["Rooms"];

                    foreach (var element in joRecive["Rooms"])
                    {
                        if (element["name"].ToString() == roomName.Text)
                        {
                            var gpJsonObject = new JObject
                            {
                                { "id", element["id"].Value<int>() }
                            };
                            data = LoginWindow.serializeMessage(gpJsonObject, GET_PLAYERS_IN_ROOM_CODE);
                            stream.Write(data, 0, data.Length);

                            buffer = new byte[1024];
                            bytesRead = stream.Read(buffer, 0, buffer.Length);
                            response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                            serverMSG = response.Substring(5);

                            joRecive = JObject.Parse(serverMSG);
                            if (joRecive.ContainsKey("PlayersInRoom"))
                            {
                                JArray joArrPlayers = (JArray)joRecive["PlayersInRoom"];
                                objWaitingRoomWindow.Admin.Content = joArrPlayers[0].ToString();
                                for (int i = 0; i < joArrPlayers.Count; i++)
                                {
                                    objWaitingRoomWindow.AddItem(joArrPlayers[i].ToString());
                                }
                                this.Visibility = Visibility.Hidden;
                                objWaitingRoomWindow.Show();
                            }
                        }

                    }
                    
                }
                else
                {
                }
            }

        }

    }
}