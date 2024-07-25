#include "UIPWorkoutView.h"

UIPWorkoutView::UIPWorkoutView()
{
    m_hidden = true;
    m_activeBar = 0;
    m_state = UIWorkoutStage::UICooldown;
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
}

UIPWorkoutView::UIPWorkoutView(UIWorkoutStage startingState)
{
    m_state = startingState;
    m_hidden = false;
    m_multiBars = true;
    m_x = c_uipwvPosition.x;
    m_y = c_uipwvPosition.y;
    m_width = c_uipwvWidth;
    m_height = c_uipwvHeight;

    float uipwvUsableArea = c_uipwvWidth - (c_uipwvPaddingHorizontal*4);
    float pbX = c_uipwvPosition.x - c_uipwvWidth/2 + c_uipwvPaddingHorizontal;
    float pbWidth = uipwvUsableArea/3;

    m_elements = new UIElement*[c_uipwvNumElements];
    for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
        UIPWVElem iElem = static_cast<UIPWVElem>(i);
        switch (iElem) {
            case BG:
                m_elements[i] = new UIImage("peloProgBG.pvr", 
                    c_uipwvPosition.x, c_uipwvPosition.y, c_uipwvWidth, c_uipwvHeight);
                break;
            case SINGLE:
                m_elements[i] = new UIProgressBar("peloProgUF.pvr", "peloProgFill.pvr",
                    c_uipwvPosition.x, c_uipwvPosition.y, c_uipwvWidth-(2*c_uipwvPaddingHorizontal), 
                    c_uipwvHeight-(2*c_uipwvPaddingVertical), 0, 0, 1);
                m_elements[i]->Hide();
                break;
            default:
                m_elements[i] = new UIProgressBar("peloProgUF.pvr", "peloProgFill.pvr", 
                    pbX + (i-1) * (uipwvUsableArea/3) + (i-1)*c_uipwvPaddingHorizontal + pbWidth/2, c_uipwvPosition.y, pbWidth, 
                    c_uipwvHeight - (2*c_uipwvPaddingVertical), 0, 0, 1);
                break;
        }
    }


}

bool
UIPWorkoutView::LoadTextures(CPVRTString* const pErrorString)
{
    for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
        if (m_elements[i] != NULL) {
            if (!m_elements[i]->LoadTextures(pErrorString)) {
                fprintf(stderr, "UIPWorkoutView texture %d load failure", i);
                return false;
            }
        }
    }
    return true;
}

void
UIPWorkoutView::BuildVertices()
{
    for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
        if (m_elements[i] != NULL) {
            m_elements[i]->BuildVertices();
        }
    }
}


bool 
UIPWorkoutView::Render(GLuint uiMVPMatrixLoc, UIPrinter* printer)
{
    if (m_hidden) {
        return true;
    }

    if (!m_multiBars) {
        for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
            UIPWVElem elem = static_cast<UIPWVElem>(i);
            switch (elem) {
                case SINGLE:
                    m_elements[i]->Show();
                    if (!m_elements[i]->Render(uiMVPMatrixLoc, printer)) {
                        fprintf(stderr, "UIPWorkoutView render %d load failure", i);
                    }
                    break;
                case BG:
                    if (!m_elements[i]->Render(uiMVPMatrixLoc, printer)) {
                        fprintf(stderr, "UIPWorkoutView render %d load failure", i);
                    }
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
        UIPWVElem elem = static_cast<UIPWVElem>(i);
        if (elem == UIPWVElem::SINGLE) {
            continue;
        }
        if (m_elements[i] != NULL) {
            if (!m_elements[i]->Render(uiMVPMatrixLoc, printer)) {
                fprintf(stderr, "UIPWorkoutView render %d load failure", i);
            }
        }
    }

    return true;
}

void
UIPWorkoutView::Update(UIMessage updateMessage)
{
    UIState dispState = updateMessage.ReadState();
    if (dispState > UIPause) {
        m_hidden = true;
    } else {
        m_hidden = false;
    }

    m_state = updateMessage.ReadWorkoutStage();

    m_multiBars = updateMessage.Read(UIMultiProg);

    float bigProgWidth = 0.5 * (c_uipwvWidth - (4*c_uipwvPaddingHorizontal));
    float smallProgWidth = 0.25 * (c_uipwvWidth - (4*c_uipwvPaddingHorizontal));
    float progHeight = c_uipwvHeight - 2*c_uipwvPaddingVertical;
    float selfPositionLeft = m_x - (m_width/2);
    float usableSpace = c_uipwvWidth - (c_uipwvPaddingHorizontal*4);

    UIProgressBar* pb1 = (UIProgressBar*)m_elements[PB1];
    UIProgressBar* pb2 = (UIProgressBar*)m_elements[PB2];
    UIProgressBar* pb3 = (UIProgressBar*)m_elements[PB3];
    UIProgressBar* single = (UIProgressBar*)m_elements[SINGLE];

    if (!m_multiBars) {
        single->Show();
        single->Update(updateMessage);
        pb1->Hide();
        pb2->Hide();
        pb3->Hide();
        return;
    } else {
        single->Hide();
        pb1->Show();
        pb2->Show();
        pb3->Show();
    }

    switch (m_state) {
        case UIWarmup:
            m_activeBar = 0;
            pb1->SetSize(bigProgWidth, progHeight);
            pb1->SetPosition(selfPositionLeft + c_uipwvPaddingHorizontal + bigProgWidth/2, m_y);
            pb1->SetCompletion(updateMessage.Read(UIFloat::UIStageProgress));
            pb2->SetSize(smallProgWidth, progHeight);
            pb2->SetPosition(selfPositionLeft + 2*c_uipwvPaddingHorizontal + 1*bigProgWidth + smallProgWidth/2, m_y);
            pb2->SetCompletion(0);
            pb3->SetSize(smallProgWidth, progHeight);
            pb3->SetPosition(selfPositionLeft + 3*c_uipwvPaddingHorizontal + 1*bigProgWidth + 3*smallProgWidth/2, m_y);
            pb3->SetCompletion(0);
            break;
        case UIWorkout:
            m_activeBar = 1;
            pb1->SetSize(smallProgWidth, progHeight);
            pb1->SetPosition(selfPositionLeft + c_uipwvPaddingHorizontal + smallProgWidth/2, m_y);
            pb1->SetCompletion(1);
            pb2->SetSize(bigProgWidth, progHeight);
            pb2->SetPosition(selfPositionLeft + 2*c_uipwvPaddingHorizontal + 1*smallProgWidth + bigProgWidth/2, m_y);
            pb2->SetCompletion(updateMessage.Read(UIStageProgress));
            pb3->SetSize(smallProgWidth, progHeight);
            pb3->SetPosition(selfPositionLeft + 3*c_uipwvPaddingHorizontal + 1*bigProgWidth + 3*smallProgWidth/2, m_y);
            pb3->SetCompletion(0);
            break;
        case UICooldown:
            m_activeBar = 3;
            pb1->SetSize(smallProgWidth, progHeight);
            pb1->SetPosition(selfPositionLeft + c_uipwvPaddingHorizontal + smallProgWidth/2, m_y);
            pb1->SetCompletion(1);
            pb2->SetSize(smallProgWidth, progHeight);
            pb2->SetPosition(selfPositionLeft + 2*c_uipwvPaddingHorizontal + 3*smallProgWidth/2, m_y);
            pb2->SetCompletion(1);
            pb3->SetSize(bigProgWidth, progHeight);
            pb3->SetPosition(selfPositionLeft + 3*c_uipwvPaddingHorizontal + 2*smallProgWidth + bigProgWidth/2, m_y);
            pb3->SetCompletion(updateMessage.Read(UIStageProgress));
            break;
        default:
            m_activeBar = 0;
            pb1->SetSize(smallProgWidth, progHeight);
            pb1->SetPosition(selfPositionLeft + c_uipwvPaddingHorizontal + smallProgWidth/2, m_y);
            pb1->SetCompletion(0);
            pb2->SetSize(smallProgWidth, progHeight);
            pb2->SetPosition(selfPositionLeft + 2*c_uipwvPaddingHorizontal + 3*smallProgWidth/2, m_y);
            pb2->SetCompletion(0);
            pb3->SetSize(bigProgWidth, progHeight);
            pb3->SetPosition(selfPositionLeft + 3*c_uipwvPaddingHorizontal + 2*smallProgWidth + bigProgWidth/2, m_y);
            pb3->SetCompletion(0);
            break;
    }
}

void
UIPWorkoutView::Hide()
{
    m_hidden = true;
}

void
UIPWorkoutView::Show()
{
    m_hidden = false;
}

void 
UIPWorkoutView::Delete()
{
    for ( int i = 0; i < c_uipwvNumElements; i ++ ) {
        if (m_elements[i] != NULL) {
            m_elements[i]->Delete();
            delete m_elements[i];
            m_elements[i] = NULL;
        }
    }
    delete [] m_elements;
}