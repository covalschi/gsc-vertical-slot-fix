// Player preview on the left of the centre column, gear slots in a vertical strip on its right.
// Nothing of the host mod is copied: its widgets are found by name and moved. When one of them
// is missing the layout stays as the host mod drew it.
class GVSF_SlotLayout
{
    const int PER_COLUMN = 8;   // eight fit above a hands panel of ~300 reference px

    protected Inventory m_Inventory;

    protected Widget m_Center;
    protected Widget m_Right;
    protected Widget m_RightRoot;
    protected Widget m_Header;
    protected Widget m_OldScroller;
    protected Widget m_Hands;
    protected Widget m_CharView;
    protected Widget m_CharPanel;
    protected Widget m_RecessTop;
    protected Widget m_RecessLow;
    protected Widget m_Row;
    protected Widget m_Strip;

    protected ref array<Widget> m_Icons;

    // Layout units, so that a new resolution only rescales them
    protected float m_PitchUnits;
    protected float m_GapUnits;

    protected float m_HandsLimit;
    protected float m_LastHands = -1;
    protected float m_LastScale;
    protected bool  m_Ready;
    protected bool  m_Moved;

    void GVSF_SlotLayout(Widget root, PlayerContainer player, Inventory inventory)
    {
        m_Inventory = inventory;
        m_Center    = root.FindAnyWidget("CenterPanel");
        m_Right     = root.FindAnyWidget("RightPanel");
        m_Hands     = root.FindAnyWidget("HandsPanel");
        m_CharView  = root.FindAnyWidget("S2_CharacterView");
        m_CharPanel = root.FindAnyWidget("CharacterPanel");
        m_RecessTop = root.FindAnyWidget("S2_ColumnCentreRecessTop");
        m_RecessLow = root.FindAnyWidget("S2_ColumnCentreRecessLow");
        if (m_Right)
        {
            m_Header      = m_Right.FindAnyWidget("SlotsHeader");
            m_OldScroller = m_Right.FindAnyWidget("ScrollerSlotsContent");
        }
        if (m_OldScroller && m_OldScroller.GetParent())
            m_RightRoot = m_OldScroller.GetParent().GetParent();

        m_Icons = new array<Widget>;
        player.GVSF_CollectSlotWidgets(m_Icons, m_Row);

        m_Ready = m_Center && m_RightRoot && m_Header && m_Hands && m_CharView && m_CharPanel && m_RecessTop && m_RecessLow && m_Row;

        Print("[GVSF] tiles=" + m_Icons.Count().ToString() + " ready=" + m_Ready.ToString());
    }

    float GetHandsLimit()
    {
        return m_HandsLimit;
    }

    void Invalidate()
    {
        m_LastHands = -1;
    }

    void Tick()
    {
        if (!m_Ready)
            return;

        float centerW;
        float centerH;
        m_Center.GetScreenSize(centerW, centerH);
        if (centerW <= 0 || centerH <= 0)
            return;

        float handsW;
        float handsH;
        m_Hands.GetScreenSize(handsW, handsH);

        float scale = GetScale();
        if (m_Moved && Math.AbsFloat(handsH - m_LastHands) < 1 && Math.AbsFloat(scale - m_LastScale) < 0.001)
            return;

        if (!m_Moved && !MoveOnce(scale))
            return;

        m_LastHands = handsH;
        m_LastScale = scale;
        Apply(centerW, centerH, handsH, scale);
    }

    protected float GetScale()
    {
        int screenW;
        int screenH;
        GetScreenSize(screenW, screenH);
        float height = screenH;
        return height / 1080.0;
    }

    // Measured once, while the tiles still sit in vanilla's row: the row's height is the pitch.
    protected bool MoveOnce(float scale)
    {
        float rowW;
        float rowH;
        m_Row.GetScreenSize(rowW, rowH);
        if (rowH <= 0)
            return false;

        m_PitchUnits = rowH / scale;
        m_GapUnits   = 2;

        m_Strip = g_Game.GetWorkspace().CreateWidgets("GSC_Vertical_Slot_Fix/gui/gvsf_slot_strip.layout", m_RightRoot);
        if (!m_Strip)
            return false;

        foreach (Widget icon : m_Icons)
        {
            m_Strip.AddChild(icon);
        }
        m_OldScroller.Show(false);

        WidgetEventHandler.GetInstance().RegisterOnDropReceived(m_Strip, m_Inventory, "OnRightPanelDropReceived");
        WidgetEventHandler.GetInstance().RegisterOnDraggingOver(m_Strip, m_Inventory, "DraggingOverRightPanel");

        m_Moved = true;
        return true;
    }

    protected void Apply(float centerW, float centerH, float handsH, float scale)
    {
        float headerW;
        float headerH;
        m_Header.GetScreenSize(headerW, headerH);

        float pitch = m_PitchUnits * scale;
        float tile  = pitch - m_GapUnits * scale;

        // The tiles never move: eight to a column, and the hands panel is the one that scrolls.
        int count = m_Icons.Count();
        int cols  = (count + PER_COLUMN - 1) / PER_COLUMN;

        float stripW = cols * pitch;
        float stripH = PER_COLUMN * pitch;
        float availH = centerH - headerH - handsH;

        m_HandsLimit = centerH - headerH - stripH;

        float centerX;
        float centerY;
        float rightX;
        float rightY;
        m_Center.GetScreenPos(centerX, centerY);
        m_Right.GetScreenPos(rightX, rightY);

        m_Strip.SetPos(centerX - rightX + centerW - stripW, centerY - rightY + headerH);
        m_Strip.SetSize(stripW, stripH);

        float inset = (pitch - tile) * 0.5;
        for (int i = 0; i < count; i++)
        {
            int col = i / PER_COLUMN;
            int row = i % PER_COLUMN;
            m_Icons[i].SetPos(col * pitch + inset, row * pitch + inset);
            m_Icons[i].SetSize(tile, tile);
        }

        // S2_CharacterView and CharacterPanel keep a width fraction and an exact height;
        // CharacterPanel is centre-referenced, so moving its left edge means half the offset.
        float widthFrac = (centerW - stripW) / centerW;
        m_CharView.SetPos(0, headerH);
        m_CharView.SetSize(widthFrac, availH);
        m_CharPanel.SetPos(-stripW * 0.5, headerH);
        m_CharPanel.SetSize(widthFrac, availH);

        // The column frame is hollow: both wells start at the panel's top, behind the title plate.
        float parentX;
        float parentY;
        m_RecessLow.GetParent().GetScreenPos(parentX, parentY);
        float leftX = centerX - parentX;
        float topY  = centerY - parentY;
        m_RecessLow.SetPos(leftX, topY);
        m_RecessLow.SetSize(centerW - stripW, centerH);
        m_RecessTop.SetPos(leftX + centerW - stripW, topY);
        m_RecessTop.SetSize(stripW, centerH);

        #ifdef DIAG_DEVELOPER
        Print("[GVSF] cols=" + cols.ToString() + " pitch=" + pitch.ToString() + " avail=" + availH.ToString() + " hands=" + handsH.ToString());
        #endif
    }
}

modded class PlayerContainer
{
    // Slot order is vanilla's: row by row, eight to a row. Hidden tiles of the last row stay put.
    void GVSF_CollectSlotWidgets(array<Widget> icons, out Widget firstRow)
    {
        int rows = m_PlayerAttachmentsContainer.Count();
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < ITEMS_IN_ROW; c++)
            {
                SlotsIcon icon = GetSlotsIcon(r, c);
                if (icon && icon.GetMainWidget().IsVisible())
                    icons.Insert(icon.GetMainWidget());
            }
        }

        if (icons.Count() > 0)
            firstRow = icons[0].GetParent();
    }
}

modded class HandsArea
{
    // Vanilla lets the hands panel grow to half the screen; here it stops under the slot strip
    // and its own scrollbar takes over.
    override void UpdateInterval()
    {
        super.UpdateInterval();

        Inventory inventory = Inventory.GetInstance();
        if (!inventory || !m_Scroller)
            return;

        float limit = inventory.GVSF_GetHandsLimit();
        if (limit <= 0)
            return;

        float w;
        float h;
        m_Scroller.GetSize(w, h);
        if (h > limit)
            m_Scroller.SetSize(w, limit);
    }
}

modded class Inventory
{
    protected ref GVSF_SlotLayout m_GVSFSlotLayout;

    void Inventory(LayoutHolder parent)
    {
        m_GVSFSlotLayout = new GVSF_SlotLayout(GetMainWidget(), m_RightArea.GetPlayerContainer(), this);
    }

    float GVSF_GetHandsLimit()
    {
        if (!m_GVSFSlotLayout)
            return 0;

        return m_GVSFSlotLayout.GetHandsLimit();
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);

        if (m_GVSFSlotLayout)
            m_GVSFSlotLayout.Tick();
    }

    override void OnShow()
    {
        super.OnShow();

        if (m_GVSFSlotLayout)
            m_GVSFSlotLayout.Invalidate();
    }
}
