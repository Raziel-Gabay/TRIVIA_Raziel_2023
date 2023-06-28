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

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for StatisticsWindow.xaml
    /// </summary>
    public partial class StatisticsWindow : Window
    {
        public const uint HIGHSCORE_CODE = 105;
        public const uint USER_STATISTICS_CODE = 106;

        public StatisticsWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;
        private void OpenHighScoresWindow(object sender, RoutedEventArgs e)
        {
            HighScoresWindow objHighScoresWindow = new HighScoresWindow(m_client, m_username);

            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, HIGHSCORE_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                Label[] usernameLabels = new Label[]
{
                    objHighScoresWindow.firstUsername,
                    objHighScoresWindow.secondUsername,
                    objHighScoresWindow.thirdUsername
                };
                Label[] scoreLabels = new Label[]
{
                    objHighScoresWindow.firstScore,
                    objHighScoresWindow.secondScore,
                    objHighScoresWindow.thirdScore
                };

                int index = 0;

                foreach (var property in joRecive["HighScores"].Children<JProperty>())
                {
              
                    string propertyName = property.Name;
                    JToken propertyValue = property.Value;

                    usernameLabels[index].Content += propertyName;
                    scoreLabels[index].Content += propertyValue.ToString();

                    index++;
                }
                this.Visibility = Visibility.Hidden;
                objHighScoresWindow.Show();
            }
            else
            {
            }
        }

        private void BackToLastWindow(object sender, RoutedEventArgs e)
        {
            MainWindow objMainWindow = new MainWindow(m_client, m_username);
            this.Visibility = Visibility.Hidden;
            objMainWindow.Show();
        }

        private void OpenUserStatisticsWindow(object sender, RoutedEventArgs e)
        {
            UserStatisticsWindow objUserStatisticsWindow = new UserStatisticsWindow(m_client, m_username);

            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, USER_STATISTICS_CODE
                );

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                Label[] labels = new Label[]
{
                    objUserStatisticsWindow.AverageAnswerTime,
                    objUserStatisticsWindow.CorrectAnswers,
                    objUserStatisticsWindow.TotalAnswers,
                    objUserStatisticsWindow.NumberOfGames,
                    objUserStatisticsWindow.Score
                };


                int index = 0;

                foreach (var element in joRecive["UserStatistics"])//.Children<JProperty>())
                {
                    string value = element.ToString();

                    labels[index].Content = value;

                    index++;
                }
                
                this.Visibility = Visibility.Hidden;
                objUserStatisticsWindow.Show();
            }
            else
            {
            }
        }

    }
}
