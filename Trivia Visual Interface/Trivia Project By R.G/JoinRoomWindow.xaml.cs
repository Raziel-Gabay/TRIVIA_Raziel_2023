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
using System.Threading;
using System.Windows.Threading;

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
        public const uint GET_ROOM_STATE_CODE = 111;
        public JoinRoomWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;
        private DispatcherTimer timer;
        private bool isTimerActive;

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

        public void roomsListUpdate(object sender, EventArgs e)
        {
            if (!isTimerActive)
            {
                return; // Exit early if the timer is not active
            }
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
                if ((int)response[0] == GET_ROOMS_CODE)
                {

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        List<string> itemsToAdd = new List<string>();

                        foreach (var element in joRecive["Rooms"])
                        {
                            if (element["isActive"].Value<bool>())
                            {
                                itemsToAdd.Add(element["name"].ToString());

                            }

                        }

                        // Create a separate list to store items to be removed
                        List<string> itemsToRemove = new List<string>();

                        // Loop through the items in the ListBox
                        foreach (string item in roomsListBox.Items)
                        {
                            // Check if the item needs to be removed
                            if (itemsToAdd.Contains(item))
                            {
                                // Add the item to the removal list
                                itemsToAdd.Remove(item);
                            }
                            else
                            {
                                itemsToRemove.Add(item);
                            }

                            // Other operations or actions with the item
                        }

                        // Remove the items from the ListBox outside the loop
                        foreach (string itemToRemove in itemsToRemove)
                        {
                             roomsListBox.Items.Remove(itemToRemove);
                        }
                        foreach (string itemToAdd in itemsToAdd)
                        {
                            AddItem(itemToAdd);
                        }

                    });
                }
            }
        }
        private void JoinRoomClick(object sender, RoutedEventArgs e)
        {
            if (roomsListBox.SelectedItem != null)
            {
                StopTimer();
                string selectedRoom = roomsListBox.SelectedItem.ToString();
                JObject emptyJson = new JObject();
                byte[] data = LoginWindow.serializeMessage(emptyJson, GET_ROOMS_CODE);
                NetworkStream stream = this.m_client.GetStream();
                stream.Write(data, 0, data.Length);

                byte[] buffer = new byte[1024];
                //Thread.Sleep(3000);
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                int firstIndex = response.IndexOf((char)GET_ROOMS_CODE);
                int length = LoginWindow.FourCharsToInt(response.Substring(firstIndex + 1, 4));

                string serverMSG = response.Substring(firstIndex + 5, length);

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
                                WaitingRoomWindow objWaitingRoomWindow = new WaitingRoomWindow(m_client, m_username);
                                objWaitingRoomWindow.updateWindowButtons(m_username);
                                this.Visibility = Visibility.Hidden;
                                objWaitingRoomWindow.Show();
                                return; // Add this line to exit the method after showing the WaitingRoomWindow
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


        private void StartThreads(object sender, EventArgs e)
        {
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(3);
            timer.Tick += roomsListUpdate;
            isTimerActive = true;
            timer.Start();
        }

        private void StopTimer()
        {
            if (timer != null)
            {
                isTimerActive = false;
                timer.Tick -= roomsListUpdate;
                timer.Stop();
                timer = null;
            }
        }
    }
}
