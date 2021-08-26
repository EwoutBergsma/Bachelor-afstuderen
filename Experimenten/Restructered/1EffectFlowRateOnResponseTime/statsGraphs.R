#install.packages("readxl")
#install.packages("gplots")

library("readxl")
library(gplots)
attach(mtcars)

data <- read_excel("S:\\UMCG\\Graduation\\Experimenten\\2FlowRate\\RawData\\combined.xlsx", skip=1)

group1 <- data.frame(data[0:165,2:6])
group1$mean <- rowMeans(group1, na.rm=TRUE)
group1$time <- data.frame(data[0:165,1])

group2 <- data.frame(data[0:61,7:11])
group2$mean <- rowMeans(group2, na.rm=TRUE)
group2$time <- data.frame(data[0:61,1])

groupActual <- c(190, 190, 589, 589)
groupActualTime <- c(0, 4.499999, 4.5, 180)



plot(data.frame(group1$time, group1$mean), type="n", xlim=range(0,60), ylim=range(150,650), xaxs="i", yaxs="i", main="Effect flow rate on response", ylab="Oxygen (hPa)", xlab="Time (s)")

lines(data.frame(groupActualTime, groupActual), type="l", col="grey", lwd=1, lty=1)

for(i in 1:5){
  lines(data.frame(group1$time, group1[i]), col="orange")
}

for(i in 1:5){
  lines(data.frame(group2$time, group2[i]), col="lightgreen")
}

lines(data.frame(group1$time, group1$mean), type="l", col="orange4", lwd=2, lty=3)
lines(data.frame(group2$time, group2$mean), type="l", col="darkgreen", lwd=2, lty=3)

legend("bottomright", legend=c("150 ml/min", "575 ml/min", "Actual", "Average"), col=c("orange", "lightgreen", "grey", "black"), lty=c(1,1,1,3), lwd=2, inset = .05, title = "Legend")


#plot(as.matrix(group1$mean)~as.matrix(group1$time))
#print(as.matrix(group1$mean))
#print(group1$mean)
#plotmeans(as.matrix(group1$mean)~as.matrix(time))

########################################################################################

RTslow <- c(56, 73, 58, 55, 66)
RTquick <- c(32, 36, 33, 36, 36)

t.test(RTslow, RTquick)

boxplot(RTslow, RTquick, names=c("150 ml/min", "575 ml/min"), outpch = NA, border = "white")


stripchart(RTslow, vertical = TRUE, method = "jitter", 
           pch = 1, col = "maroon", bg = "bisque", 
           add = TRUE)
stripchart(RTquick, vertical = TRUE, method = "jitter", 
           pch = 1, col = "maroon", bg = "bisque", 
           add = TRUE, at = 2) 
