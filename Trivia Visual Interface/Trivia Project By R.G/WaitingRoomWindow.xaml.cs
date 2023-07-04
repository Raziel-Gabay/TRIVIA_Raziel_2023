using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
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
using System.Windows.Threading;
using Newtonsoft.Json.Linq;
using System.Windows.Media.TextFormatting;
using System.Xml.Linq;
using System.IO;
using System.Threading;

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for WaitingRoomWindow.xaml
    /// </summary>
    /// 

    public partial class WaitingRoomWindow : Window
    {
        public const uint GET_PLAYERS_IN_ROOM_CODE = 104;
        public const uint CLOSE_ROOM_CODE = 109;
        public const uint START_GAME_CODE = 110;
        public const uint GET_ROOM_STATE_CODE = 111;
        public const uint LEAVE_ROOM_CODE = 112;


        public WaitingRoomWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;
        private string roomState;
        private DispatcherTimer timer;
        private bool isTimerActive;

        public void AddItem(string roomName)
        {
            if (!string.IsNullOrEmpty(roomName))
            {
                playersListBox.Items.Add(roomName);
            }
        }
        
        public void updateWindowButtons(string username)
        {
            Button closeAndLeaveButton = WaitingRoomGrid.Children.OfType<Button>().FirstOrDefault(b => b.Name == "CloseAndLeaveButton");
            Application.Current.Dispatcher.Invoke(() =>
            {
                if (username == Admin.Content.ToString())
                {
                    closeAndLeaveButton.Content = "Close Room";
                    StartGameButton.Visibility = Visibility.Visible;
                }
                else
                {
                    closeAndLeaveButton.Content = "Leave Room";
                    closeAndLeaveButton.HorizontalAlignment = HorizontalAlignment.Center;
                    closeAndLeaveButton.VerticalAlignment = VerticalAlignment.Bottom;
                    closeAndLeaveButton.Margin = new Thickness(10, 0, 0, 10);

                    Grid.SetRow(closeAndLeaveButton, 3); // Row index
                    Grid.SetColumnSpan(closeAndLeaveButton, 2);

                    StartGameButton.Visibility = Visibility.Hidden;
                }
            });

        }
        public void roomUpdate(object sender, EventArgs e)
        {
            if (!isTimerActive)
            {
                return; // Exit early if the timer is not active
            }
            NetworkStream stream = m_client.GetStream();


            // Update Room Players
            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, GET_ROOM_STATE_CODE);


            //send
            stream.Write(data, 0, data.Length);


            //read
            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);

            JObject joRecive = JObject.Parse(serverMSG);
            if (joRecive.ContainsKey("status"))
            {
                if ((int)response[0] == GET_ROOM_STATE_CODE)
                {

                    foreach (var element in joRecive)
                    {
                        /*if (element.Key == "hasGameBegun")
                        {
                            roomState = "Has Game Begun:" + element.Value.ToString() + Environment.NewLine;
                        }*/
                        if (element.Key == "players")
                        {
                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                playersListBox.Items.Clear();
                                JArray joArrPlayers = (JArray)joRecive["players"];
                                Admin.Content = joArrPlayers[0].ToString();
                                for (int i = 0; i < joArrPlayers.Count; i++)
                                {
                                    AddItem(joArrPlayers[i].ToString());

                                }
                            });
                        }

                    }
                    
                }
                else if ((int)response[0] == LEAVE_ROOM_CODE)
                {
                    JoinRoomWindow objJoinRoomWindow = new JoinRoomWindow(m_client, m_username);
                    StopTimer();
                    this.Visibility = Visibility.Hidden;
                    objJoinRoomWindow.Show();
                }
                else if ((int)response[0] == START_GAME_CODE)
                {
                    MessageBox.Show("The Admin Start The Game!");
                }
            }
        }

        private void CloseAndLeaveRoomClick(object sender, RoutedEventArgs e)
        {

            if (m_username == Admin.Content.ToString())
            {
                CreateRoomWindow objCreateRoomWindow = new CreateRoomWindow(m_client, m_username);
                JObject emptyJson = new JObject();
                byte[] data = LoginWindow.serializeMessage(emptyJson, CLOSE_ROOM_CODE);


                //send
                NetworkStream stream = m_client.GetStream();
                stream.Write(data, 0, data.Length);

                //read
                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                string serverMSG = response.Substring(5);

                JObject joRecive = JObject.Parse(serverMSG);
                if (joRecive.ContainsKey("status"))
                {
                    StopTimer();
                    this.Visibility = Visibility.Hidden;
                    objCreateRoomWindow.Show();
                }
            }
            else
            {
                JoinRoomWindow objJoinRoomWindow = new JoinRoomWindow(m_client, m_username);
                JObject emptyJson = new JObject();
                byte[] data = LoginWindow.serializeMessage(emptyJson, LEAVE_ROOM_CODE);


                //send
                NetworkStream stream = m_client.GetStream();
                stream.Write(data, 0, data.Length);

                //read
                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

                string serverMSG = response.Substring(5);

                JObject joRecive = JObject.Parse(serverMSG);
                if (joRecive.ContainsKey("status"))
                {
                    StopTimer();
                    this.Visibility = Visibility.Hidden;
                    objJoinRoomWindow.Show();
                }

            }
        }

        private void StartGameClick(object sender, RoutedEventArgs e)
        {
            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, START_GAME_CODE);


            //send
            NetworkStream stream = m_client.GetStream();
            stream.Write(data, 0, data.Length);

            //read
            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);

            JObject joRecive = JObject.Parse(serverMSG);
            if (joRecive.ContainsKey("status"))
            {
                MessageBox.Show("You Started The Game!");
            }
        }

        private void StartThreads(object sender, EventArgs e)
        {
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(3);
            timer.Tick += roomUpdate;
            isTimerActive = true;

            // Call roomUpdate once before starting the timer
            roomUpdate(sender, e);

            timer.Start();
        }

        private void StopTimer()
        {
            if (timer != null)
            {
                isTimerActive = false;
                timer.Tick -= roomUpdate; // Remove the event handler
                timer.Stop();
                timer = null;
            }
        }
    }
}
