using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace SGCTRemote
{
    [JsonObject(MemberSerialization.OptIn)]
    public class Model
    {
        public Model()
        {
 
        }

        [JsonProperty]
        private string name = "";
        [JsonProperty]
        private string fileName = "";
        [JsonProperty]
        private bool isBase = false;
        [JsonProperty]
        private bool isCanon = false;

        [JsonProperty]
        private double xPos = 0;
        [JsonProperty]
        private double yPos = 0;
        [JsonProperty]
        private double zPos = 0;

        [JsonProperty]
        private double xRot = 0;
        [JsonProperty]
        private double yRot = 0;
        [JsonProperty]
        private double zRot = 0;

        public string getName()
        {
            return name;
        }
        public string getFileName()
        {
            return fileName;
        }      

        public void setName(string theName)
        {
            name = theName;
        }
        public void setFileName(string theFileName)
        {
            fileName = theFileName;
        }

        public bool getIsBase()
        {
            return isBase;
        }

        public void setIsBase(bool theIsBase)
        {
            isBase = theIsBase;
        }

        public bool getIsCanon()
        {
            return isCanon;
        }

        public void setIsCanon(bool theIsCanon)
        {
            isCanon = theIsCanon;
        }


        public double getXpos()
        {
            return xPos;
        }
        public double getYpos()
        {
            return yPos;
        }
        public double getZpos()
        {
            return zPos;
        }

        public double getXrot()
        {
            return xRot;
        }
        public double getYrot()
        {
            return yRot;
        }
        public double getZrot()
        {
            return zRot;
        }

        public void setXpos(double theXpos)
        {
            xPos = theXpos;
        }
        public void setYpos(double theYpos)
        {
            yPos = theYpos;
        }
        public void setZpos(double theZpos)
        {
            zPos = theZpos;
        }

        public void setXrot(double theXrot)
        {
            xRot = theXrot;
        }
        public void setYrot(double theYRot)
        {
            yRot = theYRot;
        }
        public void setZrot(double theZrot)
        {
            zRot = theZrot;
        }
    }
}
