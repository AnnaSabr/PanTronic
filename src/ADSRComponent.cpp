#include "ADSRComponent.hpp"

ADSRComponent::ADSRComponent()
{
    setSize(400, 150);
    setInterceptsMouseClicks(true, false);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    
    // Hintergrund
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillRoundedRectangle(bounds, 5.0f);
    
    // Umrandung
    g.setColour(primaryColor.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    
    // Grid-Linien (optional)
    g.setColour(secondaryColor.withAlpha(0.2f));
    for (int i = 1; i < 4; ++i)
    {
        float y = bounds.getY() + (bounds.getHeight() / 4.0f) * i;
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }
    
    // ADSR-Kurve zeichnen
    auto path = createADSRPath();
    
    // Gefüllte Fläche unter der Kurve
    auto filledPath = path;
    filledPath.lineTo(bounds.getRight(), bounds.getBottom());
    filledPath.lineTo(bounds.getX(), bounds.getBottom());
    filledPath.closeSubPath();
    
    g.setColour(primaryColor.withAlpha(0.2f));
    g.fillPath(filledPath);
    
    // ADSR-Linie
    g.setColour(primaryColor);
    g.strokePath(path, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved));
    
    // Kontrollpunkte zeichnen
    auto attackPoint = getAttackPoint();
    auto decayPoint = getDecayPoint();
    auto sustainPoint = getSustainPoint();
    auto releasePoint = getReleasePoint();
    
    auto drawControlPoint = [&](juce::Point<float> point, bool isActive) {
        float radius = isActive ? 8.0f : 6.0f;
        g.setColour(isActive ? primaryColor : primaryColor.withAlpha(0.8f));
        g.fillEllipse(point.x - radius/2, point.y - radius/2, radius, radius);
        
        g.setColour(juce::Colours::white);
        g.drawEllipse(point.x - radius/2, point.y - radius/2, radius, radius, 2.0f);
    };
    
    drawControlPoint(attackPoint, currentDragMode == DragMode::Attack);
    drawControlPoint(decayPoint, currentDragMode == DragMode::Decay);
    drawControlPoint(sustainPoint, currentDragMode == DragMode::Sustain);
    drawControlPoint(releasePoint, currentDragMode == DragMode::Release);
    
    // Labels
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    
    g.drawText("A", attackPoint.x - 10, bounds.getBottom() + 5, 20, 15, juce::Justification::centred);
    g.drawText("D", decayPoint.x - 10, bounds.getBottom() + 5, 20, 15, juce::Justification::centred);
    g.drawText("S", sustainPoint.x - 10, bounds.getBottom() + 5, 20, 15, juce::Justification::centred);
    g.drawText("R", releasePoint.x - 10, bounds.getBottom() + 5, 20, 15, juce::Justification::centred);
}

void ADSRComponent::resized()
{
    // Wird aufgerufen wenn die Komponente ihre Größe ändert
}

void ADSRComponent::mouseDown(const juce::MouseEvent& event)
{
    lastMousePos = event.position;
    currentDragMode = getHitTest(event.position);
    
    if (currentDragMode != DragMode::None)
    {
        repaint();
    }
}

void ADSRComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (currentDragMode == DragMode::None)
        return;
        
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto delta = event.position - lastMousePos;
    
    const float sensitivity = 0.003f;
    
    switch (currentDragMode)
    {
        case DragMode::Attack:
            attackValue = juce::jlimit(0.01f, 1.0f, attackValue + delta.x * sensitivity);
            break;
            
        case DragMode::Decay:
            decayValue = juce::jlimit(0.01f, 1.0f, decayValue + delta.x * sensitivity);
            break;
            
        case DragMode::Sustain:
            sustainValue = juce::jlimit(0.0f, 1.0f, sustainValue - delta.y * sensitivity);
            break;
            
        case DragMode::Release:
            releaseValue = juce::jlimit(0.01f, 1.0f, releaseValue + delta.x * sensitivity);
            break;
            
        default:
            break;
    }
    
    lastMousePos = event.position;
    
    // Callback aufrufen
    if (onParameterChanged)
    {
        onParameterChanged(attackValue, decayValue, sustainValue, releaseValue);
    }
    
    repaint();
}

void ADSRComponent::mouseUp(const juce::MouseEvent& event)
{
    currentDragMode = DragMode::None;
    repaint();
}

void ADSRComponent::mouseMove(const juce::MouseEvent& event)
{
    auto hitTest = getHitTest(event.position);
    
    if (hitTest != DragMode::None)
    {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    }
    else
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }
}

void ADSRComponent::setAttack(float attack)
{
    attackValue = juce::jlimit(0.01f, 1.0f, attack);
    repaint();
}

void ADSRComponent::setDecay(float decay)
{
    decayValue = juce::jlimit(0.01f, 1.0f, decay);
    repaint();
}

void ADSRComponent::setSustain(float sustain)
{
    sustainValue = juce::jlimit(0.0f, 1.0f, sustain);
    repaint();
}

void ADSRComponent::setRelease(float release)
{
    releaseValue = juce::jlimit(0.01f, 1.0f, release);
    repaint();
}

void ADSRComponent::updateColors(juce::Colour primary, juce::Colour secondary)
{
    primaryColor = primary;
    secondaryColor = secondary;
    repaint();
}

juce::Path ADSRComponent::createADSRPath() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    juce::Path path;
    
    // Startpunkt (0, unten)
    float startX = bounds.getX();
    float startY = bounds.getBottom();
    path.startNewSubPath(startX, startY);
    
    // Attack-Phase
    auto attackPoint = getAttackPoint();
    path.lineTo(attackPoint);
    
    // Decay-Phase
    auto decayPoint = getDecayPoint();
    path.lineTo(decayPoint);
    
    // Sustain-Phase (horizontale Linie)
    auto sustainPoint = getSustainPoint();
    path.lineTo(sustainPoint);
    
    // Release-Phase
    auto releasePoint = getReleasePoint();
    path.lineTo(releasePoint);
    
    return path;
}

ADSRComponent::DragMode ADSRComponent::getHitTest(juce::Point<float> position) const
{
    const float hitRadius = 10.0f;
    
    if (getAttackPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Attack;
    if (getDecayPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Decay;
    if (getSustainPoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Sustain;
    if (getReleasePoint().getDistanceFrom(position) < hitRadius)
        return DragMode::Release;
        
    return DragMode::None;
}

juce::Point<float> ADSRComponent::getAttackPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    
    float x = bounds.getX() + (bounds.getWidth() * 0.25f * attackValue);
    float y = bounds.getY(); // Peak (oben)
    
    return {x, y};
}

juce::Point<float> ADSRComponent::getDecayPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto attackPoint = getAttackPoint();
    
    float x = attackPoint.x + (bounds.getWidth() * 0.25f * decayValue);
    float y = bounds.getY() + (bounds.getHeight() * (1.0f - sustainValue));
    
    return {x, y};
}

juce::Point<float> ADSRComponent::getSustainPoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto decayPoint = getDecayPoint();
    
    float x = bounds.getX() + bounds.getWidth() * 0.7f; // Feste Position
    float y = decayPoint.y; // Gleiche Höhe wie Decay-Ende
    
    return {x, y};
}

juce::Point<float> ADSRComponent::getReleasePoint() const
{
    auto bounds = getLocalBounds().toFloat().reduced(10);
    auto sustainPoint = getSustainPoint();
    
    float x = sustainPoint.x + (bounds.getWidth() * 0.3f * releaseValue);
    float y = bounds.getBottom(); // Zurück auf null
    
    return {x, y};
}

float ADSRComponent::mapToADSRValue(float screenValue, bool isTime) const
{
    // Einfache lineare Abbildung für diese Implementierung
    return juce::jlimit(0.0f, 1.0f, screenValue);
}

float ADSRComponent::mapFromADSRValue(float adsrValue, bool isTime) const
{
    // Einfache lineare Abbildung für diese Implementierung
    return juce::jlimit(0.0f, 1.0f, adsrValue);
}