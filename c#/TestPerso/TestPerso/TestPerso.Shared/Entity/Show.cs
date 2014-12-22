using System;
using System.Collections.Generic;
using System.Text;

namespace TestPerso.Entity
{
    public class SeasonsDetail
    {
        public int number { get; set; }
        public int episodes { get; set; }
    }

    public class Notes
    {
        public string total { get; set; }
        public string mean { get; set; }
        public int user { get; set; }
    }
    public class User
    {
        public bool archived { get; set; }
        public bool favorited { get; set; }
        public int remaining { get; set; }
        public int status { get; set; }
        public string last { get; set; }
        public object tags { get; set; }
    }

    public class Show
    {
        public int id { get; set; }
        public int thetvdb_id { get; set; }
        public string imdb_id { get; set; }
        public string title { get; set; }
        public string description { get; set; }
        public string seasons { get; set; }
        public List<SeasonsDetail> seasons_details { get; set; }
        public string episodes { get; set; }
        public string followers { get; set; }
        public string comments { get; set; }
        public string similars { get; set; }
        public string characters { get; set; }
        public string creation { get; set; }
        public List<string> genres { get; set; }
        public string length { get; set; }
        public string network { get; set; }
        public string rating { get; set; }
        public string status { get; set; }
        public string language { get; set; }
        public Notes notes { get; set; }
        public bool in_account { get; set; }
        public User user { get; set; }
        public string resource_url { get; set; }
    }

    public class RootObject
    {
        public List<Show> shows { get; set; }
        public List<object> errors { get; set; }
    }
}
