#include "shapefiltering.h"

ShapeFiltering::ShapeFiltering()
{
    TRIcount = 0;
    RECTcount = 0;
    PENTAcount = 0;
    CIRcount = 0;
    Robotcount = 0;

    checked = false;

    frameCount = 0;

    shapeDetected = false;
}

void ShapeFiltering::shapeManager(std::string type)
{
//    qDebug() << " TRI : " << TRIcount << " , RECT count : " << RECTcount << " , Cir Count :" << CIRcount << " , Penta Count :" << PENTAcount ;
//    qDebug()<<"Before change : " << QString::fromStdString(this->type);
//    qDebug()<<"1";

    if(!shapeDetected)
    {
        if(type == "TRI")
        {
            TRIcount++;
            if(TRIcount > 2000)
            {
                shapeDetected = true;
                this->type = "TRI";
                return ;
            }
        }
        else if(type == "RECT")
        {
            RECTcount++;
            if(RECTcount > 2000)
            {
                shapeDetected = true;
                this->type = "RECT";
                return ;
            }
        }
        else if(type == "PENTA")
        {
            PENTAcount++;
            if(PENTAcount > 2000)
            {
                shapeDetected = true;
                this->type = "PENTA";
                return ;
            }
        }
        else if(type == "CIR")
        {
            CIRcount++;
            if(CIRcount > 2000)
            {
                shapeDetected = true;
                this->type = "CIR";
                return ;
            }
        }
        else if(type == "Robot")
        {
            Robotcount++;
            if(Robotcount > 2000)
            {
                shapeDetected = true;
                this->type = "Robot";
                return ;
            }
        }

        shapeCount[0] = TRIcount;
        shapeCount[1] = RECTcount;
        shapeCount[2] = PENTAcount;
        shapeCount[3] = CIRcount;
        shapeCount[4] = Robotcount;
    }

        int index = -1;
        int max = 0;
        for(int i=0;i<5;i++)
        {
            if(shapeCount[i]>max)
            {
                max = shapeCount[i];
                index = i;
            }
        }

        if(index == 0)
            this->type = "TRI";
        if(index == 1)
            this->type = "RECT";
        if(index == 2)
            this->type = "PENTA";
        if(index == 3)
            this->type = "CIR";
        if(index == 4)
            this->type = "Robot";

//        qDebug()<<QString::fromStdString(this->type);

}

void ShapeFiltering::plusFrameCount()
{
    this->frameCount++;
}

void ShapeFiltering::minusFrameCount()
{
    this->frameCount--;
}
