/*
 * Created by SharpDevelop.
 * User: bert
 * Date: 08.08.2012
 * Time: 20:26
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.IO;
using TBL.EDOLL;

namespace tbl_avr_gcc_installer
{
	public enum TblAvrVersionInfoLines
	{
		VersionNumber=1,
	}
	
	public class TblAvrVersionInfo
	{
		
		private TblAvrVersionInfo()
		{
			
		}
		
		public string VersionNumber
		{
			get
			{
				return(versionNumber);
			}
		}
		
		private string versionNumber;
		
		public static TblAvrVersionInfo GetFromFile(string vPathToFile)
		{
			TblAvrVersionInfo instance = new TblAvrVersionInfo();
			
			try
			{
				if(File.Exists(vPathToFile))
				{
					StreamReader sr = new StreamReader(vPathToFile);
					
					string line;
					int lineCnt=1;
					while((line = sr.ReadLine()) != null)
					{
						switch((TblAvrVersionInfoLines)lineCnt)
						{
							case TblAvrVersionInfoLines.VersionNumber:
								string[] parts = line.Split(':');
								
								instance.versionNumber = parts[1].Trim();
							break;

						default: break;
						}
						
						lineCnt++;
					}
					
					sr.Close();
					
					
					
				}
				else
				{
					stdOut.Error("Could not open Version-Information-File @ '"+vPathToFile+"'");
					instance = null;
				}			
			}
			catch(Exception e)
			{
				stdOut.Error("Could not read Version-Information-File @ '"+vPathToFile+"'");
				stdOut.Debug(e.ToString());
				instance = null;
			}
			
			return(instance);
		}
	}
	
}
