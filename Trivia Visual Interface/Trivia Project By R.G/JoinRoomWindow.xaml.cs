using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Net.Sockets;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Windows.Markup;
using System.Text.Json.Nodes;

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        public const uint GET_ROOMS_CODE = 103;
        public const uint GET_PLAYERS_IN_ROOM_CODE = 104;
        public const uint JOIN_ROOM_CODE = 107;
        public JoinRoomWindow(TcpClient client, string username)
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

        public void AddItem(string roomName)
        {
            if (!string.IsNullOrEmpty(roomName))
            {
                roomsListBox.Items.Add(roomName);
            }
        }

        private void JoinRoomClick(object sender, RoutedEventArgs e)
        {
            if (roomsListBox.SelectedItem != null)
            {
                string selectedRoom = roomsListBox.SelectedItem.ToString();
                WaitingRoomWindow objWaitingRoomWindow = new WaitingRoomWindow(m_client, m_username);
                JObject emptyJson = new JObject();
                byte[] data = LoginWindow.serializeMessage(emptyJson, GET_ROOMS_CODE);
                NetworkStream stream = this.m_client.GetStream();
                stream.Write(data, 0, data.Length);

                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                string serverMSG = response.Substring(5);

                JObject joRecive = JObject.Parse(serverMSG);
                if (joRecive.ContainsKey("status"))
                {
                    //JArray joArrRooms = (JArray)joRecive["Rooms"];

                    foreach (var element in joRecive["Rooms"])
                    {
                        if (element["name"].ToString() == selectedRoom)
                        {

                            var jrAgpJsonObject = new JObject // join room and get players in room json
                            {
                                { "id", element["id"].Value<int>() }
                            };
                            data = LoginWindow.serializeMessage(jrAgpJsonObject, JOIN_ROOM_CODE);
                            stream.Write(data, 0, data.Length);

                            buffer = new byte[1024];
                            bytesRead = stream.Read(buffer, 0, buffer.Length);
                            response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                            serverMSG = response.Substring(5);

                            joRecive = JObject.Parse(serverMSG);
                            if (joRecive.ContainsKey("status"))
                            {
                                data = LoginWindow.serializeMessage(jrAgpJsonObject, GET_PLAYERS_IN_ROOM_CODE);
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
                            else
                            {
                                MessageBox.Show("An ERROR Occurred!");
                            }
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("No room selected.");
            }
        }
    }
}
