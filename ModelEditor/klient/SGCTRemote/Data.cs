using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace SGCTRemote
{
    [JsonObject(MemberSerialization.OptIn)]
    public class Data
    {
        public Data()
        { 
            _models = new List<Model>();
        }

        [JsonProperty]
        public string name = "";
        [JsonProperty]
        public int hp = 0;

        [JsonProperty]
        public List<Model> _models;
    }
}
